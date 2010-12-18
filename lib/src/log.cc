/*
   File name:  log.cc
   Date:       2010/03/17 03:33
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define XC_DEBUG_IDENT xc::debug::LOG

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <time.h>
#include <vector>

#include <xc/text.h>
#include "xc/log.h"
#include "xc/debug.h"
#include "xc/error.h"
#include "xc/file.h"

namespace {

    struct mask_t
    {
        int info_level;
        int warn_level;
        int error_level;

        mask_t() {
            info_level = warn_level = error_level = 9999;
        }

        mask_t(const char* str) {
            info_level = warn_level = error_level = 9999;
            set(str);
        }

        void set_all() {
            info_level = warn_level = error_level = -1;
        }

        void set(const char* str) {
            if (*str == 'A') {
                set_all();
                return;
            }

            // reset
            info_level = warn_level = error_level = 9999;
            while (*str) {
                char t = *str++;
                int level = ::atoi(str);
                if (level <= 0)
                    ERROR(xc::error_t, "Failed parse log mask level %s", str);
                while (::isdigit(*str)) str++;

                switch (t) {
                case 'i':
                case 'I':
                    info_level = level;
                    XC_DBG("Set level for log info to %d", level);
                    break;
                case 'w':
                case 'W':
                    warn_level = level;
                    XC_DBG("Set level for log warn to %d", level);
                    break;
                case 'e':
                case 'E':
                    error_level = level;
                    XC_DBG("Set level for log error to %d", level);
                    break;
                case 'f':
                case 'F':
                    if (level > 1)
                        LOG_WARN(2, "Failed suppress log fatal level to %d", level);
                    break;
                default:
                    ERROR(xc::error_t, "Unknown log type `%c'", t);
                };
            }
        }

        bool info(int level) const {
            return level >= info_level;
        }

        bool warn(int level) const {
            return level >= warn_level;
        }

        bool error(int level) const {
            return level >= error_level;
        }

    };

    size_t print_time(FILE* out)
    {
        size_t ret;
        char outstr[200];
        time_t t;
        struct tm *tmp;

        t = ::time(NULL);
        tmp = ::localtime(&t);
        if (tmp == NULL)
            return 0;

        if ((ret = ::strftime(outstr, sizeof(outstr), "[%T] ", tmp)) == 0)
            return 0;

        ::fputs(outstr, out);
        return ret;
    }

    size_t print_procinfo(FILE* out)
    {
        return ::fprintf(out, "[%d] ", ::getpid());
    }

    size_t print_info(FILE* out)
    {
        //return print_time(out) + print_procinfo(out);
        size_t s1 = print_time(out);
        s1 += print_procinfo(out);
        return s1;
    }
    
    void print_loc(FILE* out, const xc::debug::loc_t& loc)
    {
        if (loc.filename() && loc.func())
            fprintf(out, _(" at %s:%d:%s"), loc.filename(), loc.line(), loc.func());
        else if (loc.filename())
            fprintf(out, _(" at %s:%d"), loc.filename(), loc.line());
        else if (loc.func())
            fprintf(out, _(" at %s"), loc.func());
    }

    class log_mod_t
    {
    public:
        log_mod_t(const char* mask) : _mask(mask) {}
        bool info(int level) const { return _mask.info(level); }
        virtual void info(const xc::debug::loc_t& loc, int level, const xc::string& msg) = 0;

        bool warn(int level) const { return _mask.warn(level); }
        virtual void warn(const xc::debug::loc_t& loc, int level, const xc::string& msg) = 0;

        bool error(int level) const { return _mask.error(level); }
        virtual void error(const xc::debug::loc_t& loc, int level, const xc::string& msg) = 0;

        virtual void fatal(const xc::debug::loc_t& loc, const xc::string& msg) = 0;

        virtual void rotate() {}
    private:
        mask_t _mask;
    };

    typedef std::vector<log_mod_t*> mod_type;
    mod_type _mods;

    class log_std_t : public log_mod_t
    {
    public:
        log_std_t(FILE* out, const char* mask) : _out(out), log_mod_t(mask) {}

        virtual void info(const xc::debug::loc_t& loc, int level, const xc::string& msg) {
            size_t ind = print_info(_out);
            loc.trace().dump(_out, ind);
            fprintf(_out, "INFO%d: ", level, ind);
            fwrite(msg.data(), 1, msg.size(), _out);
            print_loc(_out, loc);
            fprintf(_out, "\n");
            fflush(_out);
        }

        virtual void warn(const xc::debug::loc_t& loc, int level, const xc::string& msg) {
            size_t ind = print_info(_out);
            loc.trace().dump(_out, ind);
            {
                xc::colored_t color(_out, 33);
                fprintf(_out, "WARN%d: ", level);
                fwrite(msg.data(), 1, msg.size(), _out);
            }
            print_loc(_out, loc);
            fprintf(_out, "\n");
            fflush(_out);
        }

        virtual void error(const xc::debug::loc_t& loc, int level, const xc::string& msg) {
            size_t ind = print_info(_out);
            loc.trace().dump(_out, ind);
            {
                xc::colored_t color(_out, 31);
                fprintf(_out, "ERROR%d: ", level);
                fwrite(msg.data(), 1, msg.size(), _out);
            }
            print_loc(_out, loc);
            fprintf(_out, "\n");
            fflush(_out);
        }

        virtual void fatal(const xc::debug::loc_t& loc, const xc::string& msg) {
            size_t ind = print_info(_out);
            loc.trace().dump(_out, ind);
            {
                xc::colored_t color(_out, 31);
                fprintf(_out, "FATAL: ");
                fwrite(msg.data(), 1, msg.size(), _out);
            }
            print_loc(_out, loc);
            fprintf(_out, "\nExiting...\n");
            fflush(_out);
        }
    protected:
        FILE* _out;
    };

    class log_file_t : public xc::file_t, public log_std_t
    {
    public:
        log_file_t(const char* path, const char* mask)
            : xc::file_t(path, "a"), log_std_t(this->handle(), mask) {}
        virtual void rotate() {
            XC_DBG("Rotate log %s", this->name());
            this->open(this->name(), "a");
            this->_out = this->handle();
        }
    };

} // namespace

void xc::log_info(const xc::debug::loc_t& loc, int level, const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    logv_info(loc, level, format, vl);
    va_end(vl);
}

void xc::log_warn(const xc::debug::loc_t& loc, int level, const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    logv_warn(loc, level, format, vl);
    va_end(vl);
}

void xc::log_error(const xc::debug::loc_t& loc, int level, const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    logv_error(loc, level, format, vl);
    va_end(vl);
}

void xc::log_fatal(const xc::debug::loc_t& loc, const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    logv_fatal(loc, format, vl);
    va_end(vl);
}

void xc::logv_info(const xc::debug::loc_t& loc, int level, const char* format, va_list arg)
{
    xc::string msg;
    for (mod_type::iterator i = _mods.begin(); i != _mods.end(); ++i) {
        if ((*i)->info(level)) {
            if (msg.empty())
                msg = xc::vformat(_(format), arg);
            (*i)->info(loc, level, msg);
        }
    }
}

void xc::logv_warn(const xc::debug::loc_t& loc, int level, const char* format, va_list arg)
{
    xc::string msg;
    for (mod_type::iterator i = _mods.begin(); i != _mods.end(); ++i) {
        if ((*i)->warn(level)) {
            if (msg.empty())
                msg = xc::vformat(_(format), arg);
            (*i)->warn(loc, level, msg);
        }
    }
}

void xc::logv_error(const xc::debug::loc_t& loc, int level, const char* format, va_list arg)
{
    xc::string msg;
    for (mod_type::iterator i = _mods.begin(); i != _mods.end(); ++i) {
        if ((*i)->error(level)) {
            if (msg.empty())
                msg = xc::vformat(_(format), arg);
            (*i)->error(loc, level, msg);
        }
    }
}

void xc::logv_fatal(const xc::debug::loc_t& loc, const char* format, va_list arg)
{
    xc::string msg = xc::vformat(_(format), arg);
    for (mod_type::iterator i = _mods.begin(); i != _mods.end(); ++i) {
        (*i)->fatal(loc, msg);
    }
    exit(EXIT_FAILURE);
}

void xc::log::add_file(const char* path, const char* mask)
{
    XC_DBG("Open file %s with mask %s", path, mask);
    //_m.set(mask);
    _mods.push_back(new log_file_t(path, mask));
}

void xc::log::add_stderr(const char* mask)
{
    XC_DBG("Set log to stderr with mask %s", mask);
    //_m.set(mask);
    _mods.push_back(new log_std_t(stderr, mask));
}

void xc::log::logrotate()
{
    XC_DBG("Rotate logs");
    for (mod_type::iterator i = _mods.begin(); i != _mods.end(); ++i) {
        (*i)->rotate();
    }
}

/*
   File name:  compile.cc
   Date:       2010/04/12 13:41
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/config.h>
#include <xc/log.h>
#include <xc/error.h>

#include "xc/template/compile.h"
#include "xc/template/fs.h"
#include "xc/template/scanner.h"
#include "xc/template/page.h"
#include "xc/template/preprocess.h"

namespace {

    using namespace xc::templ;

    class mkpage_t : public xc::templ::page_t
    {
    public:
        template<typename T>
        void add(const T& i) {
            _cbuff.append(reinterpret_cast<const char*>(&i), sizeof(i));
        }

        uint32_t add_data(const char* str, size_t size) {
            uint32_t ret = _dbuff.size();
            _dbuff.append(str, size);
            _dbuff.push_back('\0');
            return ret;
        }

        void add_idata(inscode_t code, const char* str, size_t size) {
            ins_data_t ins;
            ins.code = code;
            ins.data.ptr = this->add_data(str, size);
            ins.data.size = size;
            this->add(ins);
        }

        void add_ipage(const xc::string& name) {
            ins_page_t ins;
            ins.code = TI_PAGE;
            ins.name_size = name.size();
            this->add(ins);
            _cbuff.append(name);
            LOG_INFO(1, "COMPILE: add page %s", name.c_str());
        }
        void add_idict(const xc::string& name) {
            ins_dict_t ins;
            ins.code = TI_DICT;
            ins.name_size = name.size();
            this->add(ins);
            _cbuff.append(name);
            LOG_INFO(1, "COMPILE: dict %s", name.c_str());
        }

        void finish() {
            _code = (unsigned char*)(_cbuff.data());
            _data = (const char*)(_dbuff.data());
        }
    private:
        xc::string _cbuff;
        xc::string _dbuff;
    };
}

xc::templ::compiler_t::compiler_t(files_t& files)
    : _files(files)
{
}

xc::templ::compiler_t::~compiler_t()
{
}

xc::templ::page_t* xc::templ::compiler_t::compile(const char* name)
{
    scanner_t* scan = _files.open(name);
    preproc_t in(*scan);
    xc::string out;
    mkpage_t* page = new mkpage_t();
    while (scanner_t::term_t t = in.read()) {
        LOG_INFO(1, "%s", t.name());

        switch (t.type) {
        case TI_RAW:
            page->add_idata(t.type, t.text, t.length);
            break;
        case TI_PAGE:
            page->add_ipage(t.sub(2,2));
            break;
        case TI_DICT:
            page->add_idict(t.sub(2,1));
            break;
        default:
            LOG_WARN(3, "Ignore template token %s. Not implemented", t.name());
        };
    };

    ins_t e;
    e.code = TI_END;
    page->add(e);
    page->finish();
    return page;
}




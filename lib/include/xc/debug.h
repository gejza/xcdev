/*
   File name:  debug.h
   Date:       2010/03/24 22:14
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

/**
 * @example debug.cc
 */
#ifndef _XC_DEBUG_H_
#define _XC_DEBUG_H_
#pragma once

#include <stdio.h>
#include <xc/string.h>

namespace xc {
namespace debug {

/**
 * Debug type inden
 */
enum {
    MEMORY = 1,
    FS,
    NET,
    CONFIG,
    LOG,
    USER = 10000,
};

/**
 * @brief trace_t 
 */
class trace_t
{
public:

    /**
     * Default constructor
     */
    trace_t(bool trace = true);

    /**
     * Destructor
     */
    ~trace_t();

    /**
     * Copy constructor
     * @param other Reference on object to copy.
     */
    trace_t(const trace_t& other);

    /** 
     * Assignment operator
     * @param other Reference on object to copy.
     * @return Reference on initialisated object.
     */
    trace_t& operator=(const trace_t& other);

    unsigned int size() const {
        return _size;
    }

    xc::string name(unsigned int index) const;

    xc::string operator[](unsigned int index) const {
        return this->name(index);
    }

    void dump(FILE* out, size_t indent) const;

    size_t operator - (const trace_t& d) const {
        size_t s1 = _size;
        size_t s2 = d._size;
        while (s1 && s2) {
            if (_p[s1-1] != d._p[s2-1])
                break;
            s1--; s2--;
        }
        return s1;
    }
private:

    void** _p;
    unsigned int _size;
};


/**
 * @short description ...
 */
class loc_t
{
public:

    /** 
     * Default constructor
     */
    loc_t() {}

    /**
     * Destructor
     */
    virtual ~loc_t() {}

    /** 
     * @short Source Filename
     * @return filename
     */
    virtual const char* filename() const {
        return 0x0;
    }

    /** 
     * @short Source line
     * @return line
     */
    virtual int line() const {
        return 0;
    }

    /** 
     * @short Function
     * @return name
     */
    virtual const char* func() const {
        return 0x0;
    }

    /** 
     * @short 
     * @return 
     */
    virtual const trace_t& trace() const {
        static trace_t non(false);
        return non;
    }
};

/**
 * @short description ...
 */
class loc_src_t : public loc_t
{
public:

    /** 
     * Default constructor
     */
    loc_src_t()
        : _func(0x0), _filename(0x0), _line(0) {}

    /** 
     * Constructor
     * @param filename 
     * @param line 
     */
    loc_src_t(const char* filename, int line)
        : _func(0x0), _filename(filename), _line(line) {}

    /** 
     * Constructor
     * @param func 
     */
    loc_src_t(const char* func)
        : _func(func), _filename(0x0), _line(0) {}

    /** 
     * Constructor
     * @param filename 
     * @param line 
     * @param func 
     */
    loc_src_t(const char* filename, int line, const char* func)
        : _func(func), _filename(filename), _line(line) {}

    loc_src_t(const void* p);


    /**
     * Destructor
     */
    virtual ~loc_src_t() {}

    /** 
     * @short Source Filename
     * @return filename
     */
    virtual const char* filename() const {
        return _filename;
    }

    /** 
     * @short Source line
     * @return line
     */
    virtual int line() const {
        return _line;
    }

    /** 
     * @short Function
     * @return name
     */
    virtual const char* func() const {
        return _func;
    }

private:
    const char* _filename;
    int _line;
    const char* _func;
};

/**
 * @short description ...
 */
class loc_trace_t : public loc_src_t
{
public:

    /** 
     * Default constructor
     */
    loc_trace_t()
        : loc_src_t() {}

    /** 
     * Constructor
     * @param filename 
     * @param line 
     */
    loc_trace_t(const char* filename, int line)
        : loc_src_t(filename, line) {}

    /** 
     * Constructor
     * @param func 
     */
    loc_trace_t(const char* func)
        : loc_src_t(func) {}

    /** 
     * Constructor
     * @param filename 
     * @param line 
     * @param func 
     */
    loc_trace_t(const char* filename, int line, const char* func)
        : loc_src_t(filename, line, func) {}

    /**
     * Destructor
     */
    virtual ~loc_trace_t() {}

    /** 
     * @short 
     * @return 
     */
    virtual const trace_t& trace() const {
        return _trace;
    }
private:
    trace_t _trace;
};

/** 
 * @short 
 * @param name 
 * @return 
 */
xc::string demangle(const char* name);

xc::string symbol_name(const void* p);

const char* ident_name(unsigned long idn);

/** 
 * @short 
 * @param ident 
 * @param loc 
 * @param format 
 * @param ... 
 */
void print(unsigned long ident, const loc_t& loc, const char* format, ...);

/** 
 * @short 
 * @param ident 
 * @param loc 
 * @param format 
 * @param ... 
 */
void trace(unsigned long ident, const loc_t& loc, const char* format, ...);

/** 
 * @short 
 */
void debug_enable();

/** 
 * @short 
 * @param ident 
 */
void debug_enable(unsigned long ident);

/** 
 * @short 
 * @param src_ex
 * @return 
 */
void debug_enable(const char* src_ex);

/** 
 * @short 
 */
void debug_disable();

} // namespace debug
} // namespace xc

#define XC_LOC_FILE ::xc::debug::loc_src_t(__FILE__, __LINE__)
#define XC_LOC_FUNC ::xc::debug::loc_src_t(__PRETTY_FUNCTION__)
#define XC_LOC_FULL ::xc::debug::loc_src_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#define XC_LOC_TRACE ::xc::debug::loc_trace_t()

#ifndef XC_DEBUG_IDENT
#define XC_DEBUG_IDENT 0
#warning "XC_DEBUG_IDENT not defined. Default is USER"
#endif

#ifdef DEBUG
#define XC_DBG(msg...) ::xc::debug::print(XC_DEBUG_IDENT, XC_LOC_FULL, msg);
#define XC_TRACE(msg...) ::xc::debug::trace(XC_DEBUG_IDENT, XC_LOC_FULL, msg);
#else
#define XC_DBG(msg...)
#define XC_TRACE(msg...)
#endif

#endif // _XC_DEBUG_H_
/* end of debug.h */


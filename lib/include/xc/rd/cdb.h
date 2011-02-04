/*
   File name:  cdb.h
   Date:       2010/12/05 21:49
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_RD_CDB_H_
#define _XC_RD_CDB_H_
#pragma once

#include <xc/fd.h>
#include <xc/data.h>
#include <cdb.h>

#include "lookup.h"

namespace xc {
namespace rd {

class ConstDB_t : public Lookup_t
{
public:
    class Cursor_t;

    ConstDB_t(const char* fn);
    virtual ~ConstDB_t();
    
    virtual bool exist(const xc::rd::ns_t ns, const xc::data_t& key) const;
    
    virtual bool lookup(const xc::rd::ns_t ns, const xc::data_t& key,
			xc::data_t& value) const;

    Cursor_t cursor() const;

    typedef void (*Iterator_t)(const xc::rd::ns_t ns, const xc::data_t& key,
            const xc::data_t& value);
    virtual void dump(Iterator_t iter) const;
protected:
    struct cdb* _get() const {
        return const_cast<cdb*>(&this->_db);
    }

    struct cdb* _get() {
        return &this->_db;
    }

private:
    xc::fd_t _fd;
    struct cdb _db;
};

class ConstDB_t::Cursor_t
{
public:
    bool valid() const {
        return this->_db != NULL;
    }

    xc::rd::ns_t ns() const;
    const xc::data_t key() const;
    const xc::data_t value() const;

    void next();

    operator bool() const {
        return this->valid();
    }

    const Cursor_t& operator++(int) {
        printf("post");
        this->next();
        return *this;
    }

    const Cursor_t& operator++() {
        this->next();
        return *this;
    }
protected:
    struct cdb* _get() const {
        return const_cast<cdb*>(this->_db);
    }

    struct cdb* _get() {
        return this->_db;
    }

private:
    Cursor_t(struct cdb* db);

    friend Cursor_t ConstDB_t::cursor() const;

    struct cdb* _db;
    unsigned int _ctx;
};

} // namespace rd
} // namespace xc

#endif // _XC_RD_CDB_H_
/* end of cdb.h */


/*
   File name:  pogen.h
   Date:       2010/12/06 01:44
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_POGEN_H_
#define _XC_POGEN_H_
#pragma once

#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <vector>
#include <string>

namespace xc {

class POGen_t
{
public:
    struct Ref_t
    {
        std::string fn;
        int line;
    };
    typedef std::vector<Ref_t> Refs_t;
    struct Msg_t
    {
        std::string id;
        std::string plural;
        Refs_t ref;

        Msg_t(const std::string& _id) : id(_id) {}
        void addref(const std::string& fn, int line) {
            Ref_t r;
            r.fn = fn;
            r.line = line;
            ref.push_back(r);
        }
    };

    POGen_t(const char* fn);
    virtual ~POGen_t();

    void add(const char* msg, const char* plural = NULL);
    void add(const Msg_t& msg);
protected:
    void write_header();
private:
    FILE* _file;
};


} // namespace xc

#endif // _XC_POGEN_H_
/* end of pogen.h */


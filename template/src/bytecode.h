/*
   File name:  bytecode.h
   Date:       2010/02/24 19:15
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _ITISSIEN_BYTECODE_H_
#define _ITISSIEN_BYTECODE_H_
#pragma once

#include <Itissien/template.h>

namespace Itissien {
} // namespace Itissien

class Itissien::Template::IByteCode/* : public Itissien::IRefObject*/
{
public:
    IByteCode() {}
    virtual ~IByteCode() {}
    virtual const char* GetName() const = 0;

    virtual int Action() const = 0;
    virtual xc::string GetValue() const = 0;
    virtual int Next() = 0;
};

const char* TokenName(int value);

struct ByteCodeCmd
{
    Itissien::Template::IByteCode* code;
    int cmd;

    ByteCodeCmd(Itissien::Template::IByteCode* code)
        : code(code), cmd(code ? code->Action():T_END) {}

    xc::string GetString() const
    {
        return code->GetValue();
    }

    bool operator!=(int c) const { return cmd != c; }
    bool operator==(int c) const { return cmd == c; }

    const char* Name() const { return TokenName(cmd); }
    operator int() const { return cmd; }
    int operator++(int)
    {
        int ret = cmd;
        cmd = code->Next();
        return ret;
    }
    int operator++() { return cmd = code->Next(); }
};

#endif // _ITISSIEN_BYTECODE_H_
/* end of bytecode.h */


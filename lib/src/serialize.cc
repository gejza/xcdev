/*
   File name:  serialize.cc
   Date:       2010/12/12 22:49
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <iostream>

#include "xc/error.h"
#include "xc/serialize.h"
#include "xc/unserialize.h"

xc::serialize_t::serialize_t(xc::ostream_t& out)
    : _out(out) {
}

void xc::serialize_t::_write(bytecode::ChunkId_t id, const void* ptr, size_t size)
{
    bytecode::Chunk_t ch = bytecode::chunk(id, size);
    _out.write(ch);
    _out.write(ptr, size);
}

void xc::serialize_t::_writekey(const std::string& key)
{
    _write(bytecode::KEY, key.c_str(), key.size() + 1);
}
    
void xc::serialize_t::write(const std::string& str)
{
    _write(bytecode::STRING, str.c_str(), str.size() + 1);
}

void xc::serialize_t::write(const serial_t& s)
{
    _write(bytecode::ARRAY, s.str());
}
/*    void write(const std::string& str) {
    }
    void write(const xc::serialize_t& s);
   {
    }

protected:
    void _write(bytecode::ChunkId_t id, const void* ptr, size_t size) {
    }
    
    void _write(bytecode::ChunkId_t id, const std::string& str) {
        this->_write(id, str.data(), str.size());
    }*/
/*namespace {
    void 
    xc::ostream_t& _serialize(xc::ostream_t& out, const std::string& str)
}

xc::ostream_t& serialize(xc::ostream_t& out, const std::string& str)
{
    if (!str.empty()) {
        out.write(bytecode::STRING, str);
    }
    return out;
}*/

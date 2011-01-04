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

namespace {
	void write(xc::buffer_t& out,
			xc::chunk_t::id_t id, const std::string& key, const void* data, size_t size)
	{
		xc::chunk_t chunk;
		chunk._id = id;
		chunk._keysize = key.size() + 1;
		chunk._valsize = size;
		out.append(&chunk, sizeof(xc::chunk_t));
		if (chunk.key_size())
			out.append(key.c_str(), key.size() + 1);
		out.append(data, size);
	}
}

void xc::serialize(xc::buffer_t& out, const std::string& key, int value)
{
	write(out, xc::chunk_t::LONG, key, &value, sizeof(int));
	printf("Add %s=%d\n", key.c_str(), value);
}

void xc::serialize(xc::buffer_t& out, const std::string& key, const std::string& value)
{
	write(out, xc::chunk_t::STRING, key, value.c_str(), value.size());
	printf("Add %s=%s\n", key.c_str(), value.c_str());
}

void xc::serialize(xc::buffer_t& out, const std::string& key, const serialize_t& ser)
{
	write(out, xc::chunk_t::ARRAY, key, ser.data(), ser.size());
	printf("Array %s=%ld\n", key.c_str(), ser.size());
}
/*
xc::serialize_t::serialize_t(xc::ostream_t& out)
    : _out(out) {
}

void xc::serialize_t::_write(bytecode::ChunkId_t id, const void* ptr, size_t size)
{
    XC_DBG("Serialize %ld bytes as %x", size, (unsigned int)id);
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

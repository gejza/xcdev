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

xc::unserialize_t::unserialize_t(const xc::data_t& data)
    : _data(data) {
//    _key(NULL), _chunk(xc::bytecode::END), _value(_data) {
  //      this->next();
}

xc::unserialize_t::unserialize_t(const std::string& str)
    : _data(reinterpret_cast<const uint8_t*>(str.data()), str.size()) {
    //_key(NULL), _chunk(xc::bytecode::END), _value(_data) {
      //  this->next();
}

xc::unserialize_t::unserialize_t(const chunk_t& chunk)
    : _data(reinterpret_cast<const uint8_t*>(chunk.value()), chunk.value_size()) {
    //_key(NULL), _chunk(xc::bytecode::END), _value(_data) {
      //  this->next();
}
xc::unserialize_t::unserialize_t(const uint8_t* data, size_t size)
    : _data(data, size) {
}

const xc::chunk_t* xc::unserialize_t::read()
{
    if (_data.size() < sizeof(xc::chunk_t))
        return NULL;
	const xc::chunk_t* chunk = reinterpret_cast<const xc::chunk_t*>(_data.data());
    _data += chunk->size();
    return chunk;
}

#if 0


protected:

private:
    /**
     * Chunk na aktualni pozici dat
     */

    /**
     * Delka dat klice
     */
    size_t _keysize() const {
        return (bytecode::id(_chunk()) == bytecode::KEY)
            ? sizeof(bytecode::Chunk_t) + bytecode::size(_chunk())
            : 0;
    }

    template<typename Value_t>
    const Value_t& _read() {
        return *reinterpret_cast<const Value_t*>(_data.cut(sizeof(Value_t)).data());
    }

    xc::data_t _read(size_t size) {
        return _data.cut(size);
    }
#endif

//TODO: napojit na xc lib
void xc::dump(xc::unserialize_t& ser, int level)
{
    while (const xc::chunk_t* val = ser.read()) {
        if (val->key()) {
            std::cout << val->key() << " = ";
        }
        //std::cout << std::hex << ser.id() << std::endl;
        switch (val->id()) {
		case xc::chunk_t::ARRAY:
            {
                std::cout << "(" << std::endl;
                xc::unserialize_t arr(*val);
                dump(arr, level + 1);
                std::cout << ")\n" << std::endl;
            }
            break;
		case xc::chunk_t::STRING:
            std::cout << (const char*)val->value() << std::endl;
            break;
		case xc::chunk_t::LONG:
            std::cout << *((int*)val->value()) << std::endl;
            break;
        default:
            RUNTIME_ERROR("Unknown chunk id %x", (unsigned int)val->id());
            return;
        };
    }
}




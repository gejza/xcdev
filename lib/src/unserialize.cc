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

xc::unserialize_t::unserialize_t(const uint8_t* data, size_t size)
    : _data(data, size) {
}

xc::unserialize_t::value_t xc::unserialize_t::read()
{
    return xc::unserialize_t::value_t(_data);
}

xc::bytecode::Chunk_t read_chunk(xc::data_t& data)
{
    if (data.size() < sizeof(xc::bytecode::Chunk_t))
        return xc::bytecode::END;
    xc::bytecode::Chunk_t chunk =
        *reinterpret_cast<const xc::bytecode::Chunk_t*>(data.data());
    data += sizeof(xc::bytecode::Chunk_t);
    return chunk;
}

xc::unserialize_t::value_t::value_t(xc::data_t& data)
    :_chunk(xc::bytecode::END), _key(NULL),_value(data)
{
    _chunk = read_chunk(data);
    if (xc::bytecode::id(_chunk) == xc::bytecode::KEY) {
        _key = reinterpret_cast<const char*>(data.data());
        data += xc::bytecode::size(_chunk);
        _chunk = read_chunk(data);
    }
    _value = data.cut(xc::bytecode::size(_chunk));
}

xc::unserialize_t xc::unserialize_t::value_t::array() const {
    return unserialize_t(_value);
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
    while (xc::unserialize_t::value_t val = ser.read()) {
        if (val.key()) {
            std::cout << val.key() << " = ";
        }
        //std::cout << std::hex << ser.id() << std::endl;
        switch (val.id()) {
        case bytecode::ARRAY:
            {
                std::cout << "(" << std::endl;
                xc::unserialize_t arr = val.array();
                dump(arr, level + 1);
                std::cout << ")\n" << std::endl;
            }
            break;
        case bytecode::STRING:
            std::cout << val.c_str() << std::endl;
            break;
        default:
            RUNTIME_ERROR("Unknown chunk id %x", (unsigned int)val.id());
            return;
        };
    }
}




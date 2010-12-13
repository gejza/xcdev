/*
   File name:  serialize.h
   Date:       2010/12/07 15:09
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_SERIALIZE_H_
#define _XC_SERIALIZE_H_
#pragma once

#include <stdint.h>
#include <assert.h>
#include <map>
#include <vector>
#include <string>

#include "bytecode.h"
#include "stream.h"

namespace xc {

class serial_t;
/**
 * Serialize class
 */
class serialize_t
{
public:
    /**
     * Kontruktor
     * @param out Output stream
     */
    serialize_t(xc::ostream_t& out);

    /**
     * Write value with key
     */
    template<typename Object_t>
    void write(const std::string& key, const Object_t& obj) {
        this->_writekey(key);
        this->write(obj);
    }

    void write(const std::string& str);

    void write(const serial_t& s);

protected:
    void _writekey(const std::string& key);

    void _write(bytecode::ChunkId_t id, const void* ptr, size_t size);
    
    void _write(bytecode::ChunkId_t id, const std::string& str) {
        this->_write(id, str.data(), str.size());
    }
private:
    xc::ostream_t& _out;
};

class serial_t : public serialize_t
{
public:
    serial_t()
        : serialize_t(_data) {}
    
    const std::string& str() const {
        return _data.str();
    }
/*

    const uint8_t* data() const {
        return reinterpret_cast<const uint8_t*>(_data.data());
    }

    size_t size() const {
        return _data.size();
    }

    const std::string& str() const {
        return _data;
    }
*/
private:
    xc::ostream_data_t _data;
};

ostream_t& serialize(ostream_t& out, const std::string& str);
/*template<typename Value_t>
void write(const std::vector<Value_t>& objs) {
    serialize_t ser;
    for (typename std::vector<Value_t>::const_iterator
        i = objs.begin(); i != objs.end(); ++i)
       ser.write(*i);
    this->write(ser);
}*/

    /*template<typename Value_t>
    void write(const std::map<std::string, Value_t>& objs) {
        serialize_t ser;
        for (typename std::map<std::string, Value_t>::const_iterator
            i = objs.begin(); i != objs.end(); ++i)
           ser.write(i->first, i->second);
        this->write(ser);
    }*/



} // namespace xc

/**
 * @example serialize.cc
 * Use class serialize_t and Unserialize_t
 */

#endif // _XC_SERIALIZE_H_
/* end of serialize.h */


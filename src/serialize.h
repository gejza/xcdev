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

#include <assert.h>
#include <map>
#include <vector>
#include <string>

namespace xc {


class ByteCode_t
{
public:
    enum ChunkId_t {
        MASK_ID = 0xFF000000,
        MASK_SIZE = 0x00FFFFFF,

        KEY = 0x01000000,
        //DATA = 0x02000000,
        STRING = 0x03000000,
        STRID = 0x04000000,
        LONG = 0x05000000,
        FLOAT = 0x06000000,

        ARRAY = 0xFF000000,
        END = 0x00000000,
    };

protected:
    typedef uint32_t Chunk_t;

    static ChunkId_t id(Chunk_t ch) {
        return ChunkId_t(MASK_ID & ch);
    }

    static size_t size(Chunk_t ch) {
        return size_t(MASK_SIZE & ch);
    }

    static Chunk_t chunk(ChunkId_t id, size_t size) {
        assert((id & MASK_ID) == id);
        assert(size < MASK_SIZE);
        return (id & MASK_ID) | (size & MASK_SIZE);
    }
};

class Serialize_t : public ByteCode_t
{
public:
    Serialize_t() {};

    const uint8_t* data() const {
        return reinterpret_cast<const uint8_t*>(_data.data());
    }

    size_t size() const {
        return _data.size();
    }

    const std::string& str() const {
        return _data;
    }

    // structures
    void begin() {
        Chunk_t ch = ByteCode_t::chunk(ARRAY, 0);
        this->_write(ch);
    }

    void begin(const std::string& key) {
        this->_write(KEY, key);
        this->begin();
    }

    void end() {
        Chunk_t ch = ByteCode_t::chunk(END, 0);
        this->_write(ch);
    }

    // data
    void write(const std::string& str) {
        if (!str.empty()) {
            this->_write(STRING, str);
        }
    }

    template<typename Value_t>
    void write(const std::vector<Value_t>& objs) {
        this->begin();
        for (typename std::vector<Value_t>::const_iterator
            i = objs.begin(); i != objs.end(); ++i)
           this->write(*i);
        this->end();
    }

    template<typename Value_t>
    void write(const std::map<std::string, Value_t>& objs) {
        this->begin();
        for (typename std::map<std::string, Value_t>::const_iterator
            i = objs.begin(); i != objs.end(); ++i)
           this->write(i->first, i->second);
        this->end();
    }

    void write(const Serialize_t& s) {
        this->begin();
        this->write(s.str());
        this->end();
    }

    // key values
    template<typename Object_t>
    void write(const std::string& key, const Object_t& obj) {
        this->_write(KEY, key);
        this->write(obj);
    }

protected:
    void _write(ChunkId_t id, const void* ptr, size_t size) {
        Chunk_t ch = ByteCode_t::chunk(id, size);
        this->_write(ch);
        this->_write(ptr, size);
    }
    
    void _write(ChunkId_t id, const std::string& str) {
        this->_write(id, str.data(), str.size());
    }
private:
    void _write(const void* ptr, size_t size) {
        _data.append((const char*)ptr, size);
    }

    void _write(const std::string& str) {
        this->_write(str.data(), str.size());
    }

    template<typename Object_t>
    void _write(const Object_t& obj) {
        this->_write(&obj, sizeof(Object_t));
    }

    std::string _data;
};

class Unserialize_t : public ByteCode_t
{
public:
    Unserialize_t(const std::string& str)
        : _data(reinterpret_cast<const uint8_t*>(str.data())), _size(str.size()) {
    }

    Unserialize_t(const uint8_t* data, size_t size) 
        : _data(data), _size(size) {
    }

    ChunkId_t id() const {
        return ByteCode_t::id(chunk());
    }

    ChunkId_t next() {
        return ByteCode_t::id(this->_read<Chunk_t>());
    }

    std::string key() {
        const Chunk_t& chunk = this->_read<Chunk_t>();
        return this->_read(ByteCode_t::size(chunk));
    }

    std::string string() {
        const Chunk_t& chunk = this->_read<Chunk_t>();
        return this->_read(ByteCode_t::size(chunk));
    }
protected:
    Chunk_t chunk() const {
        return _size >= sizeof(Chunk_t) ? *(reinterpret_cast<const Chunk_t*>(_data)) : END;
    }

private:
    void _skip(size_t size) {
        _data += size;
        _size -= size;
    }

    template<typename Value_t>
    const Value_t& _read() {
        const Value_t* val = reinterpret_cast<const Value_t*>(_data);
        this->_skip(sizeof(Value_t));
        return *val;
    }

    std::string _read(size_t size) {
        std::string ret(reinterpret_cast<const char*>(_data), size);
        this->_skip(size);
        return ret;
    }

    const uint8_t* _data;
    size_t _size;
};

inline void dump(Unserialize_t& ser, int level = 0)
{
    bool isarr = false;
    if (ser.id() == ByteCode_t::ARRAY) {
        ser.next();
        isarr = true;
        std::cout << "(" << std::endl;
    }
    while (ser.id() != ByteCode_t::END) {
        if (ser.id() == ByteCode_t::KEY) {
            std::cout << ser.key() << " = ";
        }
        switch (ser.id()) {
        case ByteCode_t::ARRAY:
            dump(ser, level + 1);
            break;
        case ByteCode_t::STRING:
            std::cout << ser.string() << std::endl;
            break;
        default:
            printf("Unknown id %x\n", (unsigned int)ser.id());
            return;
        };
    }
    if (isarr) {
        ser.next();
        std::cout << "\n)\n" << std::endl;
    }
}

} // namespace xc

#endif // _XC_SERIALIZE_H_
/* end of serialize.h */


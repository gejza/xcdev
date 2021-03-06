/*
   File name:  data.h
   Date:       2010/12/13 01:08
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_DATA_H_
#define _XC_DATA_H_
#pragma once

//TODO
#include <string.h>

#include <xc/types.h>

namespace xc {

template<typename Value_t>
Value_t min(const Value_t a, const Value_t b) {
    return b < a ? b : a;
}

/**
 * @short data_t
 */
class data_t
{
public:
	typedef uint8_t value_t;

    /**
     * Default constructor
     */
    data_t()
        : _data(NULL), _size(0) {
    }

    data_t(const value_t* data, size_t size)
        : _data(data), _size(size) {
    }

    data_t(const char* str)
        : _data(reinterpret_cast<const value_t*>(str)), _size(::strlen(str)) {
    }

    data_t(const data_t& data)
        : _data(data._data), _size(data._size) {
    }

    data_t& operator=(const data_t& data) {
        _data = data._data;
        _size = data._size;
        return *this;
    }

    /**
     * Velikost dat.
     * @return pocet bytu
     */
    const void* data() const {
        return _data;
    }

    /**
     * Velikost dat.
     * @return pocet bytu
     */
    size_t size() const {
        return _size;
    }

    /**
     * Vraceni n-bytu.
     * @param off Pozice
     * @param size Velikost dat
     * @return Data
     */
    data_t sub(size_t off) const {
        if (off >= _size)
            //TODO: check
            return data_t(NULL, 0);
        return data_t(_data + off, _size - off);
    }

    /**
     * Vraceni n-bytu.
     * @param off Pozice
     * @param size Velikost dat
     * @return Data
     */
    data_t sub(size_t off, size_t size) const {
        if (off >= _size)
            //TODO: check
            return data_t(NULL, 0);
        return data_t(_data + off, xc::min(size, _size - off));
    }

    void shift(size_t size) {
        _data += size;
        _size -= size;
    }

    /**
     * Podobne jako sub ale vnitrni ukazatel se posune na konec techto dat.
     * @param size Velikost dat
     * @return Data
     */
    data_t cut(size_t size) {
        if (size > _size)
            size = _size;
        data_t ret(this->sub(0, size));
        this->shift(size);
        return ret;
    }

    operator std::string() const {
        return std::string(reinterpret_cast<const char*>(_data), _size);
    }

    data_t& operator += (size_t size) {
        this->shift(size);
        return *this;
    }

    template<typename Value_t>
    data_t& operator >> (Value_t& value)
    {
        //TODO: check
        if (sizeof(Value_t) > _size)
            return *this;
        value = *reinterpret_cast<const Value_t*>(_data);
        this->shift(sizeof(Value_t));
        return *this;
    }

private:
    const value_t* _data;
    size_t _size;
};


} // namespace xc

#endif // _XC_DATA_H_
/* end of data.h */


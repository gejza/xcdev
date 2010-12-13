/*
   File name:  stream.h
   Date:       2010/03/30 15:00
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_STREAM_H_
#define _XC_STREAM_H_
#pragma once

namespace xc {

/**
 * @short istream_t
 */
class istream_t
{
public:

    /**
     * Default constructor
     */
    istream_t() {}

    /**
     * Destructor
     */
    virtual ~istream_t() {}

    virtual ssize_t read(char* buf, size_t nbyte) = 0;
private:
    istream_t(const istream_t&);
    istream_t& operator=(const istream_t&);
};

/**
 * @short stream_t
 */
class const_stream_t : public istream_t
{
public:

    /**
     * Default constructor
     */
    const_stream_t(const uint8_t* data, size_t size)
        : _data(data), _size(size) {
    }

    /**
     * Destructor
     */
    virtual ~const_stream_t() {}

    virtual ssize_t read(char* buf, size_t nbyte) = 0;
private:
    const_stream_t(const const_stream_t&);
    const_stream_t& operator=(const const_stream_t&);

    const uint8_t* _data;
    size_t _size;
};

/**
 * @short ostream_t
 */
class ostream_t
{
public:

    /**
     * Default constructor
     */
    ostream_t() {}

    /**
     * Destructor
     */
    virtual ~ostream_t() {}

    virtual void write(const void* buf, size_t size) = 0;
    
    template<typename Object_t>
    void write(const Object_t& obj) {
        this->write(&obj, sizeof(Object_t));
    }

private:
    ostream_t(const ostream_t&);
    ostream_t& operator=(const ostream_t&);
};

class ostream_data_t : public ostream_t
{
public:
    ostream_data_t() {}

    virtual ~ostream_data_t() {}

    virtual void write(const void* buf, size_t nbyte) {
        _out.append((const char*)buf, nbyte);
    }

    const std::string& str() const {
        return _out;
    }
private:
    ostream_data_t(const ostream_data_t&);
    ostream_data_t& operator=(const ostream_data_t&);

    std::string _out;
};

class ostream_file_t : public ostream_t
{
public:
    ostream_file_t(FILE* file)
        : _file(file) {}

    virtual ~ostream_file_t() {}

    virtual void write(const char* buf, size_t nbyte) {
        ::fwrite(buf, nbyte, 1, _file);
    }
private:
    ostream_file_t(const ostream_file_t&);
    ostream_file_t& operator=(const ostream_file_t&);

    FILE* _file;
};


} // namespace xc

#endif // _XC_STREAM_H_
/* end of stream.h */


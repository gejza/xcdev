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

    virtual void write(const char* buf, size_t nbyte) = 0;
private:
    ostream_t(const ostream_t&);
    ostream_t& operator=(const ostream_t&);
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


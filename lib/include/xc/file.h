/*
   File name:  file.h
   Date:       2010/03/25 10:24
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_FILE_H_
#define _XC_FILE_H_
#pragma once

#include <stdio.h>
#include "string.h"

namespace xc {

/**
 * @brief file_t
 */
class file_t
{
public:

    /**
     * Default constructor
     */
    file_t();

    /**
     * Constructor
     */
    explicit file_t(FILE* file);

    /**
     * Constructor
     */
    file_t(const char* filename, const char* mode = "r");

    /**
     * Destructor
     */
    ~file_t();

    /**
     * Constructor
     */
    void open(const char* filename, const char* mode = "r");

    /** 
     * @short 
     * @return 
     */
    FILE* handle() {
        return _file;
    }

    /** 
     * @short 
     * @return 
     */
    const char* name() const {
        return _name.c_str();
    }

private:
    file_t(const file_t& other);
    file_t& operator=(const file_t& other);

    xc::string _name;
    FILE* _file;
};



} // namespace xc

#endif // _XC_FILE_H_
/* end of file.h */


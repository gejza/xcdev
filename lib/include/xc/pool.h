/*
   File name:  pool.h
   Date:       2010/03/24 20:53
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_POOL_H_
#define _XC_POOL_H_
#pragma once

#include <stddef.h>

namespace xc {

/**
 * @brief pool_t ... short description ...
 * @author Milan Dunghubel <milan@ministryofnet.com>
 * @date 2010-03-24
 * ... description ...
 */
class pool_t
{
public:

    /**
     * Default constructor
     */
    pool_t();

    /**
     * Destructor
     */
    virtual ~pool_t();

    /** 
     * @short Allocate new block
     * @param size Size of block
     * @return Pointer to block
     */
    void* alloc(size_t size);

    /** 
     * @short 
     */
    void free();

    /** 
     * @short 
     * @return 
     */
    size_t used_size() const {
        return _used;
    }

    /** 
     * @short 
     * @return 
     */
    size_t alloc_size() const {
        return _alloc;
    }

    /** 
     * @short 
     * @param src 
     * @param size 
     * @return 
     */
    void* copy(const void* src, size_t size);

    /** 
     * @short 
     * @param str 
     * @param len 
     * @return 
     */
    const char* strdup(const char* str, size_t len = 0);
private:
    pool_t(const pool_t& other);
    pool_t& operator=(const pool_t& other);

    struct _chunk_t;

    _chunk_t* _new_chunk(size_t initsize);

    _chunk_t*   _ch;
    size_t      _used;
    size_t      _alloc;
};

} // namespace xc

#endif // _XC_POOL_H_
/* end of pool.h */


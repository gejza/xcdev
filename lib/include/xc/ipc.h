/*
   File name:  ipc.h
   Date:       2011/02/17 00:32
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_IPC_H_
#define _XC_IPC_H_
#pragma once

#include <mm.h>

namespace xc {

/** 
 * @short Shared memmory pool holder.
 */
class shpool_t {
public:
    /** 
     * @short 
     */
    shpool_t(size_t maxsize);

    /** 
     * @short 
     */
    ~shpool_t();

    /** 
     * @short Allocate shared block
     * @param size 
     * @param zero 
     * @return pointer
     */
    void* alloc(size_t size, bool zero = false);

    /** 
     * @short 
     * @param zero 
     * @return 
     */
    template<typename value_t>
    value_t* alloc(bool zero = false) {
        return reinterpret_cast<value_t*>(this->alloc(sizeof(value_t), zero));
    }

    /** 
     * @short 
     * @param ptr 
     */
    void free(void* ptr);

    /** 
     * @short 
     * @return 
     */
    size_t maxsize() const;

    /** 
     * @short 
     * @return 
     */
    size_t available() const;
private:
    shpool_t(const shpool_t &);
    shpool_t &operator=(const shpool_t &);

    pid_t _owner;
};

} // namespace xc

#endif // _XC_IPC_H_
/* end of ipc.h */


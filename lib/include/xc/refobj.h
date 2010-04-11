/*
   File name:  refobj.h
   Date:       2010/03/30 14:37
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_REFOBJ_H_
#define _XC_REFOBJ_H_
#pragma once

namespace xc {


/**
 * @short refobj_t
 */
class refobj_t
{
public:

    /**
     * Default constructor
     */
    refobj_t();

    /**
     * Destructor
     */
    virtual ~refobj_t();

    virtual unsigned int addref() {
        return ++_refcount;
    }

    virtual unsigned int release() {
        if (_refcount)
            _refcount--;

        if (!_refcount)
            delete this;

        return _refcount;
    }
private:
    refobj_t(const refobj_t&);
    refobj_t& operator=(const refobj_t&);

    unsigned int _refcount;
};



} // namespace xc

#endif // _XC_REFOBJ_H_
/* end of refobj.h */


/*
   File name:  dir.h
   Date:       2010/03/25 10:28
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_DIR_H_
#define _XC_DIR_H_
#pragma once

namespace xc {

/**
 * @brief dir_t
 */
class dir_t
{
public:

    /**
     * Default constructor
     */
    dir_t();

    /**
     * Destructor
     */
    ~dir_t();

private:
    dir_t(const dir_t& other);
    dir_t& operator=(const dir_t& other);
};


} // namespace xc

#endif // _XC_DIR_H_
/* end of dir.h */


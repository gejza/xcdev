/*
   File name:  fd.h
   Date:       2010/12/05 23:04
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _FD_H_
#define _FD_H_
#pragma once

class FD_t
{
public:
    FD_t(const char* fn, bool create = false);
    ~FD_t();
    int get() {
        return _fd;
    }
private:
    int _fd;
};


#endif // _FD_H_
/* end of fd.h */


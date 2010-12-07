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

namespace xc {

class Serialize_t
{
public:
    void add(const void* ptr, size_t size) {
        _data.append((const char*)ptr, size);
    }
    const std::string& str() const {
        return _data;
    }
private:
    std::string _data;
};

} // namespace xc

#endif // _XC_SERIALIZE_H_
/* end of serialize.h */


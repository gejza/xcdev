
/*
* Copyright
*/

#pragma once
#ifndef _XC_STRING_H_
#define _XC_STRING_H_

#include <string>

namespace xc {

typedef ::std::string string;
#if 0
class string : public ::std::string
{
public:
    string(const char* str)
        : ::std::string(str) {}
};
#endif

} // namespace xc

#endif // _XC_STRING_H_

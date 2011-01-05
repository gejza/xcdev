/*
   File name:  buffer.h
   Date:       2010/12/30 22:59
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan.dunghubel@gmail.com>

   Copyright (C) 2010 Milan Dunghubel <milan.dunghubel@gmail.com>
*/

#ifndef _XC_BUFFER_H_
#define _XC_BUFFER_H_
#pragma once

#include <string>

#include "data.h"

namespace xc {

    class buffer_t
    {
    public:
        typedef uint8_t value_t;

        buffer_t() {}
        ~buffer_t() {}

        const value_t* data() const {
            return _data.data();
        }

        size_t size() const {
            return _data.size();
        }

        void append(const void* ptr, size_t size) {
            _data.append(reinterpret_cast<const value_t*>(ptr), size);
        }

		operator const xc::data_t()
		{
			return xc::data_t(this->data(), this->size());
		}

		buffer_t& operator << (const xc::data_t& data)
		{
			this->append(data.data(), data.size());
			return *this;
		}

		buffer_t& operator << (char c)
		{
			this->append(&c, 1);
			return *this;
		}

		buffer_t& operator << (const char* str)
		{
			this->append(str, ::strlen(str));
			return *this;
		}
    private:
        std::basic_string<value_t> _data;
    };


} // namespace xc

#endif // _XC_BUFFER_H_
/* end of buffer.h */


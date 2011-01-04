/*
   File name:  bytecode.h
   Date:       2010/12/12 23:14
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_BYTECODE_H_
#define _XC_BYTECODE_H_
#pragma once

#include "assert.h"

namespace xc {

	struct chunk_t
	{
		enum {
			//DATA = 0x02000000,
			STRING = 0x0300,
			STRID = 0x0400,
			LONG = 0x0500,
			FLOAT = 0x0600,

			ARRAY = 0xFF00,
			END = 0x0000,
		};

		typedef uint16_t id_t;

		uint16_t _id;
		uint16_t _keysize;
		uint32_t _valsize;

		id_t id() const {
			return _id;
		}

		const char* key() const {
			return _keysize
				? reinterpret_cast<const char*>(this) + sizeof(chunk_t)
				: NULL;
		}

		size_t key_size() const {
			return _keysize;
		}

		const void* value() const {
			return reinterpret_cast<const uint8_t*>(this)
				+ sizeof(chunk_t) + key_size();
		}

		size_t value_size() const {
			return _valsize;
		}

		size_t size() const {
			return sizeof(chunk_t) + key_size() + value_size();
		}
	};


} // namespace xc

#endif // _XC_BYTECODE_H_
/* end of bytecode.h */


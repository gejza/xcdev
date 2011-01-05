/*
   File name:  unserialize.h
   Date:       2010/12/13 00:22
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_UNSERIALIZE_H_
#define _XC_UNSERIALIZE_H_
#pragma once

#include <stdint.h>
#include <assert.h>
#include <map>
#include <vector>
#include <string>

#include "data.h"
#include "bytecode.h"

namespace xc {

/**
 * Deserialize class
 */
class unserialize_t
{
public:
    /**
     * Kontruktor
     * @param data Data
     */
    unserialize_t(const xc::data_t& data);

    unserialize_t(const std::string& str);
    
	unserialize_t(const chunk_t& chunk);

    unserialize_t(const uint8_t* data, size_t size);

    const chunk_t* read();
private:
    xc::data_t _data;
};

/**
 * Dump serialized data
 */
void dump(unserialize_t& ser, int level = 0);
void dump(const chunk_t& ser, int level = 0);

} // namespace xc

/**
 * @example serialize.cc
 * Use class serialize_t and unserialize_t
 */

#endif // _XC_UNSERIALIZE_H_
/* end of unserialize.h */


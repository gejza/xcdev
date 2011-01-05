/*
 * File name: utils.h
 * Date:      2010/12/13 06:24
 * Author:    
 */

#ifndef UTILS_H
#define UTILS_H

#include <xc/unserialize.h>
#include <xc/serialize.h>

namespace xc {
	void serialize(xc::buffer_t& out, const std::string& key, zval* val);
	void serialize(xc::buffer_t& out, const std::string& key, HashTable* arr);

	void load(zval* val, xc::unserialize_t& ser);
	void load(zval* val, const xc::chunk_t& chunk);
}



#endif

/* end of utils.h */

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

#include <stdint.h>
#include <assert.h>
#include <map>
#include <vector>
#include <string>

#include "bytecode.h"
#include "stream.h"
#include "buffer.h"

namespace xc {

	class serialize_t;

	void serialize(xc::buffer_t& out, const std::string& key, int value);
	void serialize(xc::buffer_t& out, const std::string& key, const std::string& value);
	void serialize(xc::buffer_t& out, const std::string& key, const serialize_t& ser);

	template<typename Value_t>
	void serialize(xc::buffer_t& out, const Value_t& value) {
		serialize(out, std::string(), value);
	}

	template<typename Value_t>
	xc::buffer_t serialize(const Value_t& value) {
		xc::buffer_t out;
		serialize(out, value);
		return out;
	}

	/**
	 * Serialize class
	 */
	class serialize_t : public xc::buffer_t
	{
	public:
		template<typename Value_t>
		void add(const Value_t& value) {
			serialize(*this, value);
		}

		template<typename Key_t, typename Value_t>
		void add(const Key_t& key, const Value_t& value) {
			serialize(*this, std::string(key), value);
		}

	protected:
	};

	template<typename Value_t>
	void serialize(xc::buffer_t& out, const std::map<std::string, Value_t>& value) {
		xc::serialize_t obj;
		for (typename std::map<std::string, Value_t>::const_iterator i = value.begin();
				i != value.end(); i++) {
			obj.add(i->first, i->second);
		}
		serialize(out, obj);
	}

	template<typename Value_t>
	void serialize(xc::buffer_t& out, const std::vector<Value_t>& value) {
		xc::serialize_t obj;
		for (typename std::vector<Value_t>::const_iterator i = value.begin();
				i != value.end(); i++) {
			obj.add(*i);
		}
		serialize(out, obj);
	}

} // namespace xc

/**
 * @example serialize.cc
 * Use class serialize_t and Unserialize_t
 */

#endif // _XC_SERIALIZE_H_
/* end of serialize.h */


/*
   File name:  bytecode.cc
   Date:       2011/01/04 11:58
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/error.h"
#include "xc/bytecode.h"

const xc::chunk_t& xc::chunk(const xc::data_t& data) {
	static chunk_t end = {0};
	if (!data.size())
		return end;
	const chunk_t& ret = *reinterpret_cast<const chunk_t*>(data.data());
	if (data.size() < sizeof(chunk_t))
		RUNTIME_ERROR("Missing data %zu", data.size());
	if (data.size() != ret.size())
		RUNTIME_ERROR("Mismatch size (avail: %zu, chunk: %zu)", data.size(), ret.size());
	return ret;
}


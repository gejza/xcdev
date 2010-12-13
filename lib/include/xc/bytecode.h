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
namespace bytecode {

    enum ChunkId_t {
        MASK_ID = 0xFF000000,
        MASK_SIZE = 0x00FFFFFF,

        KEY = 0x01000000,
        //DATA = 0x02000000,
        STRING = 0x03000000,
        STRID = 0x04000000,
        LONG = 0x05000000,
        FLOAT = 0x06000000,

        ARRAY = 0xFF000000,
        END = 0x00000000,
    };

    typedef uint32_t Chunk_t;

    inline ChunkId_t id(Chunk_t ch) {
        return ChunkId_t(MASK_ID & ch);
    }

    inline size_t size(Chunk_t ch) {
        return size_t(MASK_SIZE & ch);
    }

    inline Chunk_t chunk(ChunkId_t id, size_t size) {
        assert((id & MASK_ID) == id);
        assert(size < MASK_SIZE);
        return (id & MASK_ID) | (size & MASK_SIZE);
    }
    
} // namespace bytecode

namespace bc = bytecode;
} // namespace xc

#endif // _XC_BYTECODE_H_
/* end of bytecode.h */


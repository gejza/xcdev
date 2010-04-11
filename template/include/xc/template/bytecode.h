/*
   File name:  bytecode.h
   Date:       2010/03/25 18:39
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_BYTECODE_H_
#define _XC_TEMPLATE_BYTECODE_H_
#pragma once

#include <stdint.h>

namespace xc {
namespace templ {

// struct align
#pragma pack(push, 4)

typedef uint32_t inscode_t;

/**
 * Term instruction
 */
enum {
    // end of code
    TI_END = 0,
    
    // output data
    TI_RAW,
    TI_TEXT,

    TI_VALUE,
    TI_DICT,

    TI_PAGE,

    TI_FRAG,

    TI_DEBUG,

    TI_FRAG_BEGIN,
    TI_FRAG_END,

    TI_SECTION,
    TI_INCLUDE,
};

/**
 * Section
 */
enum {
    TS_CODE = 1,
    TS_DATA,
    TS_DEBUG,
};

/** 
 * @short Instruction name
 * @param term Code
 * @return Term name
 */
const char* ti_name(inscode_t term);

struct ins_t
{
    uint32_t code;

    const char* name() const {
        return ti_name(code);
    }
};

struct ins_raw_t : public ins_t
{
    uint16_t size;
};

#pragma pack(pop)





} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_BYTECODE_H_
/* end of bytecode.h */


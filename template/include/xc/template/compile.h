/*
   File name:  compile.h
   Date:       2010/03/26 21:15
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_COMPILE_H_
#define _XC_TEMPLATE_COMPILE_H_
#pragma once

namespace xc {
namespace templ {

class files_t;
class page_t;

/**
 * @short compiler_t
 */
class compiler_t
{
public:

    /**
     * Default constructor
     */
    compiler_t(files_t& files);

    /**
     * Destructor
     */
    ~compiler_t();

    /** 
     * @short 
     * @param name 
     * @return 
     */
    page_t* compile(const char* name);

private:
    compiler_t(const compiler_t&);
    compiler_t& operator=(const compiler_t&);

    files_t& _files;
};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_COMPILE_H_
/* end of compile.h */


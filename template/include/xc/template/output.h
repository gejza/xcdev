/*
   File name:  output.h
   Date:       2010/03/30 14:54
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_OUTPUT_H_
#define _XC_OUTPUT_H_
#pragma once

namespace xc {

// forward
class ostream_t;

namespace templ {

/**
 * @short output_t
 */
class output_t
{
public:

    /**
     * Default constructor
     */
    output_t();

    /**
     * Destructor
     */
    virtual ~output_t();

private:
    output_t(const output_t&);
    output_t& operator=(const output_t&);
};


/**
 * @short html_output_t
 */
class html_output_t : public output_t
{
public:

    /**
     * Default constructor
     */
    html_output_t(xc::ostream_t& out);

    /**
     * Destructor
     */
    virtual ~html_output_t();

private:
    html_output_t(const html_output_t&);
    html_output_t& operator=(const html_output_t&);

    xc::ostream_t& _out;
};


} // namespace templ
} // namespace xc

#endif // _XC_OUTPUT_H_
/* end of output.h */


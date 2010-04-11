/*
   File name:  regex.h
   Date:       2010/03/25 07:05
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_REGEX_H_
#define _XC_REGEX_H_
#pragma once

namespace xc {

/**
 * @brief regex_t 
 */
class regex_t
{
public:
    /**
     * Default constructor
     */
    regex_t() : _re(0x0) { }

    /**
     * Constructor
     */
    explicit regex_t(const char* regex) : _re(0x0) {
        this->_compile(regex);
    }

    /**
     * Destructor
     */
    ~regex_t();

    /**
     * Copy constructor
     * @param other Reference on object to copy.
     */
    regex_t(const regex_t& other);

    /** 
     * Assignment operator
     * @param other Reference on object to copy.
     * @return Reference on initialisated object.
     */
    regex_t& operator=(const regex_t& other);

    /** 
     * Assignment operator
     * @param other Reference on object to copy.
     * @return Reference on initialisated object.
     */
    regex_t& operator=(const char* regex) {
        this->_compile(regex);
    }

    /** 
     * @short 
     * @param str 
     * @return 
     */
    bool exec(const char* str, size_t len = 0) const;

    /** 
     * @short 
     * @param str 
     * @return 
     */
    bool exec(const xc::string& str) const {
        return this->exec(str.data(), str.size());
    }

    /** 
     * @short 
     * @param str 
     * @return 
     */
    template<typename T>
    bool operator == (const T& str) const {
        return exec(str);
    }

    /** 
     * @short 
     * @param str 
     * @return 
     */
    template<typename T>
    bool operator != (const T& str) const {
        return !exec(str);
    }

protected:
    void _compile(const char* regex);
private:
    void* _re;
};


} // namespace xc

#endif // _XC_REGEX_H_
/* end of regex.h */


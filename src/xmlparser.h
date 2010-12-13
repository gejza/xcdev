/*
   File name:  xmlparser.h
   Date:       2010/12/06 04:58
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_XMLPARSER_H_
#define _XC_XMLPARSER_H_
#pragma once

#include <libxml++/libxml++.h>
#include "cbmake.h"
#include "obj.h"

class XMLParser_t
{
public:
    XMLParser_t(xc::CBMake_t& cb);
    ~XMLParser_t();
    void parse(const char* fn);
protected:

    void process(const xmlpp::Node* node);
    void process(const xmlpp::Node::NodeList& list);
    void process(const xmlpp::Element* node);

    void process_menu(const xmlpp::Element* node);
    void process_form(const xmlpp::Element* node);
    void process_template(const xmlpp::Element* node);

private:
    xc::CBMake_t& _cb;
    std::string _fn;
};

#endif // _XC_XMLPARSER_H_
/* end of xmlparser.h */

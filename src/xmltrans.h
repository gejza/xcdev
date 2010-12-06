/*
   File name:  xmltrans.h
   Date:       2010/12/06 14:31
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_XMLTRANS_H_
#define _XC_XMLTRANS_H_
#pragma once

#include "cbdef.h"
    
enum NodeTypeId_t
{
    NODE_UNKNOWN = 0,
    NODE_MODULE,
    NODE_THEME,
    NODE_REGISTRY,
    NODE_TEMPLATE,
    NODE_MENU,
    NODE_ITEM,
    NODE_FORM,

    NODE_INCLUDE,
    NODE_SCRIPT,
};

NodeTypeId_t type_id(const xmlpp::Node* node);

bool process_object(NodeTypeId_t type, const xmlpp::Node* node, xc::Callback_t& call);
bool process_object(NodeTypeId_t type, const xmlpp::Node* node, xc::Template_t& templ);
bool process_object(NodeTypeId_t type, const xmlpp::Node* node, xc::Menu_t& menu);

template<typename Object_t>
void process_child(const xmlpp::Node* node, Object_t& obj)
{
    if (!process_object(type_id(node), node, obj)) {
        _xml::error(node, "Element `%s' not allowed here.", node->get_name().c_str());
    }
}

#endif // _XC_XMLTRANS_H_
/* end of xmltrans.h */


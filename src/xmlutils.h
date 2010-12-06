/*
   File name:  xmlutils.h
   Date:       2010/12/06 14:12
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_XMLUTILS_H_
#define _XC_XMLUTILS_H_
#pragma once

#include <libxml++/libxml++.h>

namespace _xml {

    void error(const xmlpp::Node* node, const char* fmt, ...);
    const char* get(const xmlpp::Element* node, const char* name);
    const char* req(const xmlpp::Element* node, const char* name);
    std::string value(const xmlpp::Node* node);
    
    inline bool is_white(const xmlpp::Node* node)
    {
        const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
        return (nodeComment || (nodeText && nodeText->is_white_space()));
    }

    template<typename Object_t>
    void foreach(const xmlpp::Element* node, Object_t& obj, void (*func)(const xmlpp::Node*, Object_t& obj))
    {
        const xmlpp::Node::NodeList& list = node->get_children();
        for(xmlpp::Node::NodeList::const_iterator iter = list.begin(); iter != list.end(); ++iter)
        {
            if (!is_white(*iter)) {
                func(*iter, obj);
            }
        }
    }

} // namespace _xml

#endif // _XC_XMLUTILS_H_
/* end of xmlutils.h */


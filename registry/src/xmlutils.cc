/*
   File name:  xmlutils.cc
   Date:       2010/12/06 14:12
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "StdAfx.h"

#include "xmlutils.h"

void _xml::error(const xmlpp::Node* node, const char* fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    std::string buf = xc::vformat(fmt, ap);
    va_end (ap);
    std::string info = xc::format("%d: ", node->get_line());
    throw std::runtime_error(info + buf);
}

const char* _xml::get(const xmlpp::Element* node, const char* name)
{
    const xmlpp::Attribute* attr = node->get_attribute(name);
    if (!attr)
    {
        const xmlpp::Node::NodeList& list = node->get_children(name);
        for(xmlpp::Node::NodeList::const_iterator iter = list.begin(); iter != list.end(); ++iter)
        {
            std::cout << (*iter)->get_name() << std::endl;
            if ((*iter)->get_name() == name)
            {
                const xmlpp::TextNode* t = node->get_child_text();
                return t->get_content().c_str();
            }
        }
        return NULL;
    }
    return attr->get_value().c_str();
}

const char* _xml::req(const xmlpp::Element* node, const char* name)
{
    const char* val = get(node, name);
    if (!val)
        error(node, "Missing attribute `%s'", name);
    return val;
}

std::string _xml::value(const xmlpp::Node* node)
{
    if (const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node)) {
        return nodeText->get_content();
    }

    std::string ret;
    const xmlpp::Node::NodeList& list = node->get_children();
    for(xmlpp::Node::NodeList::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        ret += value(*iter);
    }
    return ret;
}




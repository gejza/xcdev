/*
   File name:  xmltrans.cc
   Date:       2010/12/06 14:31
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "StdAfx.h"

#include "xmlutils.h"
#include "xmltrans.h"

namespace {
    struct NodeType_t
    {
        NodeTypeId_t id;
        const char* name;
        //void (XMLParser_t::*cb)();

        bool operator==(const std::string& n)
        {
            return n == name;
        }
    };

    NodeType_t _node_type[] = {
        { NODE_REGISTRY , "registry" },
        { NODE_MODULE , "module" },
        { NODE_THEME , "theme" },
        { NODE_TEMPLATE , "template" },
        { NODE_MENU , "menu" },
        { NODE_ITEM , "item" },
        { NODE_FORM , "form" },

        { NODE_INCLUDE , "include" },
        { NODE_SCRIPT , "script" },
        { NODE_UNKNOWN, NULL },
    };

    const NodeType_t* node_type(const std::string& name)
    {
        for (const NodeType_t* t = _node_type; t->id; ++t)
        {
            if (name == t->name)
                return t;
        }
        return NULL;
    }
}

NodeTypeId_t type_id(const xmlpp::Node* node)
{
    const NodeType_t* type = ::node_type(node->get_name());
    if (!type)
        _xml::error(node, "Unknown nodetype `%s'", node->get_name().c_str());
    return type->id;
}

xc::Menu_t::Item_t menu_item(const xmlpp::Element* node)
{
    xc::Menu_t::Item_t item;
    item.title = _xml::req(node, "title");
    item.uri = _xml::req(node, "uri");
    return item;
}

bool process_object(NodeTypeId_t type, const xmlpp::Node* node, xc::Callback_t& call)
{
    switch (type)
    {
    case NODE_SCRIPT:
        call.script = _xml::value(node);
        return true;
    case NODE_INCLUDE:
        call.include.push_back(_xml::value(node));
        return true;
    default:
        return false;
    };
}

bool process_object(NodeTypeId_t type, const xmlpp::Node* node, xc::Template_t& templ)
{
    if (process_object(type, node, templ.call))
        return true;
    return false;
}

bool process_object(NodeTypeId_t type, const xmlpp::Node* node, xc::Menu_t& menu)
{
    switch (type)
    {
    case NODE_ITEM:
        //std::cout << "Child " << node->get_name() << std::endl;
        menu.items.push_back(menu_item(dynamic_cast<const xmlpp::Element*>(node)));
        return true;
    default:
        return false;
    };
}





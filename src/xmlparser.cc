/*
   File name:  xmlparser.cc
   Date:       2010/12/06 04:59
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "StdAfx.h"

#include "xmlparser.h"
#include "utils.h"

namespace {
    enum NodeTypeId_t
    {
        NODE_UNKNOWN = 0,
        NODE_REGISTRY,
        NODE_TEMPLATE,
        NODE_MENU,
        NODE_ITEM,
        NODE_FORM,
    };

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
        { NODE_TEMPLATE , "template" },
        { NODE_MENU , "menu" },
        { NODE_ITEM , "item" },
        { NODE_FORM , "form" },
        { NODE_UNKNOWN, NULL },
    };

    NodeTypeId_t node_type(const std::string& name)
    {
        for (NodeType_t* t = _node_type; t->id; ++t)
        {
            if (*t == name)
                return t->id;
        }
    }

    NodeTypeId_t node_type(const xmlpp::Element* node)
    {
        return node_type(node->get_name());
    }
}

/////////////////////////////////////////////////////////
XMLParser_t::XMLParser_t(xc::CBMake_t& cb)
    : _cb(cb)
{
}

XMLParser_t::~XMLParser_t()
{
}

void XMLParser_t::parse_error(const xmlpp::Node* node, const char* fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    std::string buf = xc::vformat(fmt, ap);
    va_end (ap);
    std::string info = xc::format("%s:%d: ", _fn.c_str(), node->get_line());
    throw std::runtime_error(info + buf);
}

void XMLParser_t::parse_error(const char* fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    std::string buf = xc::vformat(fmt, ap);
    va_end (ap);
    throw std::runtime_error(buf);
}

void XMLParser_t::parse(const char* fn)
{
    // Set the global C++ locale to the user-configured locale,
    // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
    std::locale::global(std::locale(""));

    _fn = fn;
    xmlpp::DomParser parser;
    //parser.set_validate();
    parser.set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.
    parser.parse_file(_fn);
    if(parser)
    {
        //Walk the tree:
        const xmlpp::Node* pNode = parser.get_document()->get_root_node(); //deleted by DomParser.
        this->process(pNode);
    }
}

void XMLParser_t::process(const xmlpp::Node* node)
{

    const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
    const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
    const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

    if(nodeComment)
    {
        //std::cout << "comment = " << nodeComment->get_content() << std::endl;
        return;
    }
    if(nodeText && nodeText->is_white_space()) //Let's ignore the indenting - you don't always want to do this.
        return;
    
    const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
    process(nodeElement);
    return;

  //Treat the various node types differently: 
  if(nodeText)
  {
    std::cout << "text = \"" << nodeText->get_content() << "\"" << std::endl;
  }
  else if(nodeContent)
  {
    std::cout << "content = " << nodeContent->get_content() << std::endl;
  }
  else if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
  {
    //A normal Element node:

    //line() works only for ElementNodes.
    std::cout << "     line = " << node->get_line() << std::endl;

    //Print attributes:
    const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
    for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
    {
      const xmlpp::Attribute* attribute = *iter;

      const Glib::ustring namespace_prefix = attribute->get_namespace_prefix();
      if(namespace_prefix.empty())
        std::cout << "  Attribute " << attribute->get_name() << " = " << attribute->get_value() << std::endl; 
      else
        std::cout << "  Attribute " << namespace_prefix  << ":" << attribute->get_name() << " = " << attribute->get_value() << std::endl;
    }
  } 
}

void XMLParser_t::process(const xmlpp::Node::NodeList& list)
{
    for(xmlpp::Node::NodeList::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        process(*iter);
    }
}

void XMLParser_t::process(const xmlpp::Element* node)
{
    switch (node_type(node))
    {
    case NODE_REGISTRY:
        process(node->get_children());
        return;
    case NODE_TEMPLATE:
        //process_template(node);
        return;
    case NODE_MENU:
        menu(node);
        return;
    default:
        parse_error(node, "Unknown nodetype `%s'", node->get_name().c_str());
        return;
    }
}

const char* XMLParser_t::get_value(const xmlpp::Element* node, const char* name)
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

const char* XMLParser_t::req_value(const xmlpp::Element* node, const char* name)
{
    const char* val = get_value(node, name);
    if (!val)
        parse_error(node, "Missing attribute `%s'", name);
    return val;
}

void XMLParser_t::menu(const xmlpp::Element* node)
{
    xc::Menu_t menu;
    const char* id = get_value(node, "id");
    if(id)
        menu.id = id;

    const xmlpp::Node::NodeList& list = node->get_children("item");
    for(xmlpp::Node::NodeList::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        menu.items.push_back(menu_item(dynamic_cast<const xmlpp::Element*>(*iter)));
    }

    _cb.add(menu);
    /*const Glib::ustring nodename = node->get_name();

    const Glib::ustring namespace_prefix = node->get_namespace_prefix();
    if(namespace_prefix.empty())
        std::cout << "Node name = " << nodename << std::endl;
    else
        std::cout << "Node name = " << namespace_prefix << ":" << nodename << std::endl;
    */

}
    
xc::Menu_t::Item_t XMLParser_t::menu_item(const xmlpp::Element* node)
{
    xc::Menu_t::Item_t item;
    item.title = req_value(node, "title");
    item.uri = req_value(node, "uri");
    return item;
}




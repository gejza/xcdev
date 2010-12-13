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
#include "xmlutils.h"
#include "xmltrans.h"

/////////////////////////////////////////////////////////
XMLParser_t::XMLParser_t(xc::CBMake_t& cb)
    : _cb(cb)
{
}

XMLParser_t::~XMLParser_t()
{
}

void XMLParser_t::parse(const char* fn)
{
    LOG_INFO(3, "Start parse xml file `%s'", fn);
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
    std::cout << "Process " << node->get_name() << std::endl;
    switch (type_id(node))
    {
    case NODE_MODULE:
    case NODE_THEME:
    case NODE_REGISTRY:
        process(node->get_children());
        return;
    case NODE_TEMPLATE:
        process_template(node);
        return;
    case NODE_MENU:
        process_menu(node);
        return;
    case NODE_FORM:
        process_form(node);
        return;
    default:
        _xml::error(node, "Element `%s' not allowed here.", node->get_name().c_str());
        return;
    }
}

void XMLParser_t::process_menu(const xmlpp::Element* node)
{
    xc::Menu_t menu;
    const char* id = _xml::get(node, "id");
    if(id)
        menu.id = id;

    _xml::foreach<xc::Menu_t&>(node, menu, process_child);
    _cb.add(menu);
    /*const Glib::ustring nodename = node->get_name();

    const Glib::ustring namespace_prefix = node->get_namespace_prefix();
    if(namespace_prefix.empty())
        std::cout << "Node name = " << nodename << std::endl;
    else
        std::cout << "Node name = " << namespace_prefix << ":" << nodename << std::endl;
    */

}
    
void XMLParser_t::process_form(const xmlpp::Element* node)
{

}

void XMLParser_t::process_template(const xmlpp::Element* node)
{
    xc::Template_t templ;
    templ.id = _xml::req(node, "id");
    // node 
    _xml::foreach<xc::Template_t&>(node, templ, process_child);
    _cb.add(templ);
}



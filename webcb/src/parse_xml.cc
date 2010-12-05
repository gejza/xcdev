/*
   File name:  parse_xml.cc
   Date:       2010/12/03 15:47
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan.dunghubel@gmail.com>

   Copyright (C) 2010 Milan Dunghubel <milan.dunghubel@gmail.com>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//#include <libxml/parser.h>
//#include <libxml/tree.h>
#include <libxml++/libxml++.h>

#include "build.h"

#include <iostream>

void print_indentation(unsigned int indentation)
{
  for(unsigned int i = 0; i < indentation; ++i)
    std::cout << " ";
}

void print_node(const xmlpp::Node* node, unsigned int indentation = 0)
{
  std::cout << std::endl; //Separate nodes by an empty line.
  
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
  const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

  if(nodeText && nodeText->is_white_space()) //Let's ignore the indenting - you don't always want to do this.
    return;
    
  const Glib::ustring nodename = node->get_name();

  if(!nodeText && !nodeComment && !nodename.empty()) //Let's not say "name: text".
  {
    print_indentation(indentation);

    const Glib::ustring namespace_prefix = node->get_namespace_prefix();
    if(namespace_prefix.empty())
      std::cout << "Node name = " << nodename << std::endl;
    else
      std::cout << "Node name = " << namespace_prefix << ":" << nodename << std::endl;
  }
  else if(nodeText) //Let's say when it's text. - e.g. let's say what that white space is.
  {
    print_indentation(indentation);
    std::cout << "Text Node" << std::endl;
  }

  //Treat the various node types differently: 
  if(nodeText)
  {
    print_indentation(indentation);
    std::cout << "text = \"" << nodeText->get_content() << "\"" << std::endl;
  }
  else if(nodeComment)
  {
    print_indentation(indentation);
    std::cout << "comment = " << nodeComment->get_content() << std::endl;
  }
  else if(nodeContent)
  {
    print_indentation(indentation);
    std::cout << "content = " << nodeContent->get_content() << std::endl;
  }
  else if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
  {
    //A normal Element node:

    //line() works only for ElementNodes.
    print_indentation(indentation);
    std::cout << "     line = " << node->get_line() << std::endl;

    //Print attributes:
    const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
    for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
    {
      const xmlpp::Attribute* attribute = *iter;
      print_indentation(indentation);

      const Glib::ustring namespace_prefix = attribute->get_namespace_prefix();
      if(namespace_prefix.empty())
        std::cout << "  Attribute " << attribute->get_name() << " = " << attribute->get_value() << std::endl; 
      else
        std::cout << "  Attribute " << namespace_prefix  << ":" << attribute->get_name() << " = " << attribute->get_value() << std::endl;
    }

    const xmlpp::Attribute* attribute = nodeElement->get_attribute("title");
    if(attribute)
    {
      std::cout << "title found: =" << attribute->get_value() << std::endl;
    }
  }
  
  if(!nodeContent)
  {
    //Recurse through child nodes:
    xmlpp::Node::NodeList list = node->get_children();
    for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
    {
      print_node(*iter, indentation + 2); //recursive
    }
  }
}


#if 0
const xmlChar* parse_text(xmlAttr* attr)
{
    xmlNode* text = attr->children;
    if (!text)
        return (xmlChar*)"";
    return text->content;
}

void parse_xml(xmlNode* node, mfis::reg::Build_t& build)
{
    for (; node; node = node->next) {
        printf("<%d\n", node->type);
        if (node->type == XML_ELEMENT_NODE) {
            printf("Element: %s\n", node->name);
            xmlAttr* attr = node->properties;
            for (;attr;attr = attr->next)
            {
                printf(" %s: %s\n", attr->name, parse_text(attr));
                parse_xml(attr->children, build);
            }
        }
        parse_xml(node->children, build);
    }
}
#endif

void parse_xml(const char* fn, mfis::reg::Build_t& build)
{
#if 0
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /*parse the file and get the DOM */
    doc = xmlReadFile(fn, NULL, 0);
    if (doc == NULL) {
        printf("error: could not parse file %s\n", fn);
        return;
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    parse_xml(root_element, build);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
#endif
  // Set the global C++ locale to the user-configured locale,
  // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
  std::locale::global(std::locale(""));

  std::string filepath = fn;
 
  try
  {
    xmlpp::DomParser parser;
    //parser.set_validate();
    parser.set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.
    parser.parse_file(filepath);
    if(parser)
    {
      //Walk the tree:
      const xmlpp::Node* pNode = parser.get_document()->get_root_node(); //deleted by DomParser.
      print_node(pNode);
    }
  }
  catch(const std::exception& ex)
  {
    std::cout << "Exception caught: " << ex.what() << std::endl;
  }

}




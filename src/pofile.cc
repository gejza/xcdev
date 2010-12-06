/*
   File name:  pofile.cc
   Date:       2010/12/06 00:35
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <error.h>
#include <errno.h>
#include <iostream>

#include "pofile.h"

namespace {
  void xerror(int severity,
                  po_message_t message,
                  const char *filename, size_t lineno, size_t column,
                  int multiline_p, const char *message_text)
  {
  }
  /* Signal a problem that refers to two messages.
     Similar to two calls to xerror.
     If possible, a "..." can be appended to MESSAGE_TEXT1 and prepended to
     MESSAGE_TEXT2.  */
  void xerror2(int severity,
                   po_message_t message1,
                   const char *filename1, size_t lineno1, size_t column1,
                   int multiline_p1, const char *message_text1,
                   po_message_t message2,
                   const char *filename2, size_t lineno2, size_t column2,
                   int multiline_p2, const char *message_text2)
  {
  }

    static po_xerror_handler err = { xerror, xerror2 };
}

xc::POFile_t::POFile_t()
{
    _file = po_file_create();
    if (_file == NULL)
        error (EXIT_FAILURE, errno, "couldn't create the PO file");
}

xc::POFile_t::POFile_t(const char* fn)
{
    _file = po_file_read(fn, &err);
    if (_file == NULL)
        error (EXIT_FAILURE, errno, "couldn't open the PO file %s", fn);
}

xc::POFile_t::~POFile_t()
{
    po_file_free(_file);
}

void xc::POFile_t::read()
{
    const char * const *domains = po_file_domains (_file);
    const char * const *domainp;
    for (domainp = domains; *domainp; domainp++)
         {
           const char *domain = *domainp;
                 std::cout << "Domain " << domain << std::endl;
            const char * head = po_file_domain_header(_file, domain);
                 std::cout << "Head " << head << std::endl;
           po_message_iterator_t iterator = po_message_iterator (_file, domain);
     
           for (;;)
             {
               po_message_t message = po_next_message (iterator);
     
               if (message == NULL)
                 break;
               {
                const char *msgid = po_message_msgid (message);
                 const char *msgstr = po_message_msgstr (message);
    
                 std::cout << msgid << ":" << msgstr << std::endl;
               }
             }
           po_message_iterator_free (iterator);
        //   break;
    }
}


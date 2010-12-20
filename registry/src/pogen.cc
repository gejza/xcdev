/*
   File name:  pogen.cc
   Date:       2010/12/06 01:45
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>

http://www.gnu.org/software/hello/manual/gettext/PO-Files.html

white-space
#  translator-comments
#. extracted-comments
#: reference...
#, flag...
#| msgid previous-untranslated-string
msgid untranslated-string
msgstr translated-string

#: lib/error.c:116
msgid "Unknown system error"
msgstr "Error desconegut del sistema"

white-space
#  translator-comments
#. extracted-comments
#: reference...
#, flag...
#| msgctxt previous-context
#| msgid previous-untranslated-string
msgctxt context
msgid untranslated-string
msgstr translated-string

white-space
#  translator-comments
#. extracted-comments
#: reference...
#, flag...
#| msgid previous-untranslated-string-singular
#| msgid_plural previous-untranslated-string-plural
msgid untranslated-string-singular
msgid_plural untranslated-string-plural
msgstr[0] translated-string-case-0
...
msgstr[N] translated-string-case-n

#: src/msgcmp.c:338 src/po-lex.c:699
#, c-format
msgid "found %d fatal error"
msgid_plural "found %d fatal errors"
msgstr[0] "s'ha trobat %d error fatal"
msgstr[1] "s'han trobat %d errors fatals"

*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gettext-po.h>

#include "pogen.h"

///////////////////////////////////////////
POGen_t::POGen_t(const char* fn)
{
    _file = ::fopen(fn, "wt");
    if (_file == NULL)
        error (EXIT_FAILURE, errno, "couldn't create the POT file %s", fn);
    write_header();
}

POGen_t::~POGen_t()
{
    ::fclose(_file);
}

void POGen_t::add(const char* text, const char* plural)
{
    Msg_t msg(text);
    if (plural)
        msg.plural = plural;
    this->add(msg);
}

void POGen_t::add(const Msg_t& msg)
{
    ::fprintf(_file, "\n");
//#  translator-comments
//#. extracted-comments
//#: reference...
    if (!msg.ref.empty()) {
        ::fprintf(_file, "#:");
        for (Refs_t::const_iterator i = msg.ref.begin(); i!= msg.ref.end(); i++) {
            ::fprintf(_file, " %s:%d", i->fn.c_str(), i->line);
        }
        ::fprintf(_file, "\n");
    }
//#, flag...
//#| msgid previous-untranslated-string
    ::fprintf(_file, "msgid \"%s\"\n", msg.id.c_str());
    if (msg.plural.empty())
    {
        ::fprintf(_file, "msgstr \"\"\n");
    } else {
        ::fprintf(_file, "msgid_plural \"%s\"\n", msg.plural.c_str());
        ::fprintf(_file, "msgstr[0] \"\"\n");
        ::fprintf(_file, "msgstr[1] \"\"\n");
    }
}

void POGen_t::write_header()
{
    const char head[] =
        "# SOME DESCRIPTIVE TITLE.\n"
        "# Copyright (C) YEAR THE PACKAGE'S COPYRIGHT HOLDER\n"
        "# This file is distributed under the same license as the PACKAGE package.\n"
        "# FIRST AUTHOR <EMAIL@ADDRESS>, YEAR.\n"
        "#\n"
        "#, fuzzy\n"
        "msgid \"\"\n"
        "msgstr \"\"\n"
        "\"Project-Id-Version: PACKAGE VERSION\\n\"\n"
        "\"Report-Msgid-Bugs-To: \\n\"\n"
        "\"POT-Creation-Date: 2010-12-06 02:54+0100\\n\"\n"
        "\"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\\n\"\n"
        "\"Last-Translator: FULL NAME <EMAIL@ADDRESS>\\n\"\n"
        "\"Language-Team: LANGUAGE <LL@li.org>\\n\"\n"
        "\"Language: \\n\"\n"
        "\"MIME-Version: 1.0\\n\"\n"
        "\"Content-Type: text/plain; charset=CHARSET\\n\"\n"
        "\"Content-Transfer-Encoding: 8bit\\n\"\n"
        "\"Plural-Forms: nplurals=2; plural=n>1;\\n\"\n";
    ::fwrite(head, 1, sizeof(head)-1, _file);
}

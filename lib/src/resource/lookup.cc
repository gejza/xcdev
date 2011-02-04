/*
   File name:  reglookup.cc
   Date:       2010/12/20 03:30
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "StdAfx.h"

#include <xc/log.h>
#include <xc/error.h>

#include <xc/unserialize.h>

#include "../include/xc/rd/cdb.h"

#define HISTORY "~/.xcrdlookup"

#ifdef HAVE_LIBREADLINE
#  if defined(HAVE_READLINE_READLINE_H)
#    include <readline/readline.h>
#  elif defined(HAVE_READLINE_H)
#    include <readline.h>
#  else /* !defined(HAVE_READLINE_H) */
extern char *readline ();
#  endif /* !defined(HAVE_READLINE_H) */
char *cmdline = NULL;
#else /* !defined(HAVE_READLINE_READLINE_H) */
 /* no readline */
static const char* readline(const char*) {
    static xc::string line;
    std::cin >> line;
    return line.c_str();
}
#endif /* HAVE_LIBREADLINE */

#ifdef HAVE_READLINE_HISTORY
#  if defined(HAVE_READLINE_HISTORY_H)
#    include <readline/history.h>
#  elif defined(HAVE_HISTORY_H)
#    include <history.h>
#  else /* !defined(HAVE_HISTORY_H) */
extern void add_history ();
extern int write_history ();
extern int read_history ();
#  endif /* defined(HAVE_READLINE_HISTORY_H) */
 /* no history */
#endif /* HAVE_READLINE_HISTORY */

xc::rd::ConstDB_t* db = NULL;
xc::rd::ns_t ns = 0;

#ifdef HAVE_LIBREADLINE
/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
char* key_generator (const char* text, int state)
{
    static xc::rd::ConstDB_t::Cursor_t cursor = db->cursor();
    static int list_index, len;

    /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
    if (!state)
    {
        if (db)
            cursor = db->cursor();
      list_index = 0;
      len = strlen (text);
    }

    while (++cursor) {
        if (ns && cursor.ns() != ns)
            continue;
        const xc::string key(cursor.key());
        //std::cout << xc::human(cursor.key(), 20) << std::endl;
        if (key.substr(0, len) == text) {
            return strdup(key.c_str());
        }
    }
    /* Return the next name which partially matches from the command list. */
    /*while (name = commands[list_index].name)
    {
      list_index++;

      if (strncmp (name, text, len) == 0)
        return (dupstr(name));
    }*/

    /* If no names matched, then return NULL. */
    return ((char *)NULL);
}

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
char** key_completion (const char* text, int start, int /*end*/)
{
  char **matches;
  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if (start == 0)
    matches = rl_completion_matches (text, key_generator);

  return (matches);
}
#endif /* HAVE_LIBREADLINE */

int main(int argc, const char* argv[])
{
    xc::debug::debug_enable();
    xc::log::add_stderr("ALL");

    if (argc <= 1)
    {
        fprintf(stderr, "Usage: %s <db> [ns] [key]\n", argv[0]);
        return EXIT_FAILURE;
    }

#ifdef HAVE_LIBREADLINE
    /* Allow conditional parsing of the ~/.inputrc file. */
    rl_readline_name = "XCRDLookup";
    /* Tell the completer that we want a crack first. */
    rl_attempted_completion_function = key_completion;
#endif /* HAVE_LIBREADLINE */
#ifdef HAVE_READLINE_HISTORY
    ::using_history();
    ::read_history(HISTORY);
#endif /* HAVE_READLINE_HISTORY */
    
    try {
        xc::rd::ConstDB_t lookup(argv[1]);
        db = &lookup;
		int default_ns = argc >= 3 ? atoi(argv[2]) : 0;
		const char* default_key = argc >= 4 ? argv[3] : "";

        ns = default_ns;
        if (!ns) {
            std::cin >> ns;
        }
        do {
            /* This holds all the state for our line editor */
            xc::string key = default_key;
            if (key.empty()) {
                const char* line = ::readline("test> ");
                if (!line)
                    break;
#ifdef HAVE_READLINE_HISTORY
                ::add_history(line);
#endif /* HAVE_READLINE_HISTORY */
                key = line;
                while (*key.rend() == ' ')
                    key.erase(key.end()-1);
            }
            if (!ns || key.empty()) {
                break;
            }
            std::cout << " ==== " << ns << ":" <<  key << " ==== " << std::endl;

            xc::data_t val;
            if (lookup.lookup(ns, key.c_str(), val)) {
                xc::dump(xc::chunk(val));
            } else {
                std::cout << "XXXXXXXXXXXXXX" << std::endl;
            }

        } while (argc < 4);

    } catch (const xc::error_t& e) {
        std::cerr << "Error: " << e.message() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "Std exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return EXIT_FAILURE;
    }
    
#ifdef HAVE_READLINE_HISTORY
    ::write_history(HISTORY);
#endif /* HAVE_READLINE_HISTORY */

    return EXIT_SUCCESS;
}






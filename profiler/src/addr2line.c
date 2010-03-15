/* addr2line.c -- convert addresses to line number and function name
   Copyright 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2006, 2007
   Free Software Foundation, Inc.
   Contributed by Ulrich Lauther <Ulrich.Lauther@mchp.siemens.de>

   This file is part of GNU Binutils.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#if 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfd.h>
#include "addr2line.h"
#include "demangle.h"

static bfd_boolean unwind_inlines;	/* -i, unwind inlined functions. */
static bfd_boolean base_names;		/* -s, strip directory names.  */

static asymbol **syms;		/* Symbol table.  */


/* Print a usage message to STREAM and exit with STATUS.  */
 void
 bfd_fatal(const char *string )
   {
     const char *errmsg = bfd_errmsg (bfd_get_error ());
     if (string)
       fprintf (stderr, "Addr2Line: %s: %s\n", string, errmsg);
     else
       fprintf (stderr, "Addr2Line: %s\n", errmsg);
   }
 
 void
 bfd_nonfatal(const char *string )
   {
     const char *errmsg = bfd_errmsg (bfd_get_error ());
     if (string)
       fprintf (stderr, "Addr2Line: %s: %s\n", string, errmsg);
     else
       fprintf (stderr, "Addr2Line: %s\n", errmsg);
   }

/* Process a file.  Returns an exit value for main().  */
 Addr2Line::Addr2Line (const char* file_name)
   {
  bfd_init();
  //set_default_bfd_target ();
  //cplus_demangle_set_style (style);

  const char *section_name = 0;
  const char *target = 0;
  asection *section;
  char **matching;

  //todo stat if (get_file_size (file_name) < 1)
  //  return 1;

  abfd = bfd_openr (file_name, target);
  if (abfd == NULL)
    bfd_fatal (file_name);

  if (bfd_check_format (abfd, bfd_archive))
    printf("%s: cannot get addresses from archive", file_name);

  if (! bfd_check_format_matches (abfd, bfd_object, &matching))
    {
      bfd_nonfatal (bfd_get_filename (abfd));
      if (bfd_get_error () == bfd_error_file_ambiguously_recognized)
	{
	  //todo list_matching_formats (matching);
	  free (matching);
	}
      xexit (1);
    }

  if (section_name != NULL)
    {
      section = bfd_get_section_by_name (abfd, section_name);
      if (section == NULL)
	printf("%s: cannot find section %s", file_name, section_name);
    }
  else
    section = NULL;

  slurp_symtab ();

   }

 Addr2Line::~Addr2Line ()
   {
	  if (syms != NULL)
	    {
	      free (syms);
	      syms = NULL;
	    }

	  bfd_close (abfd);
   }

     bool Addr2Line::lookup(void* addr, AddrInfo* info)
{
        asection *section = NULL; 
          sprintf(info->hex, "%x", addr);
	  pc = bfd_scan_vma (info->hex, NULL, 16);

      found = FALSE;
      if (section)
	find_offset_in_section (section);
      else
	bfd_map_over_sections (abfd, find_address_in_section, this);

      if (! found)
	{
		strcpy(info->method_name, "??");
	    strcpy(info->file_name, "??");
            info->line = 0;
	}
      else
	{
	  //do {
		const char *name;
		char *alloc = NULL;

		name = functionname;
		if (name == NULL || *name == '\0')
		  name = "??";
		else
		  {
		    alloc = cplus_demangle (name, DMGL_ANSI | DMGL_PARAMS);
		    if (alloc != NULL)
		      name = alloc;
		  }

		strcpy(info->method_name, name);

		if (alloc != NULL)
		  free (alloc);

	    if (base_names && filename != NULL)
	      {
		char *h;

		h = strrchr (filename, '/');
		if (h != NULL)
		  filename = h + 1;
	      }

	    strcpy(info->file_name, filename ? filename : "??");
            info->line = line;
	    /*todo if (!unwind_inlines)
	      found = FALSE;
	    else
	      found = bfd_find_inliner_info (abfd, &filename, &functionname, &line);
            */
	  //} while (found);

	}
   return found;
}

/* Read in the symbol table.  */

void
Addr2Line::slurp_symtab ()
{
  long symcount;
  unsigned int size;

  if ((bfd_get_file_flags (abfd) & HAS_SYMS) == 0)
    return;

  symcount = bfd_read_minisymbols (abfd, FALSE, (void**)&syms, &size);
  if (symcount == 0)
    symcount = bfd_read_minisymbols (abfd, TRUE /* dynamic */, (void **) &syms, &size);

  if (symcount < 0)
    bfd_fatal (bfd_get_filename (abfd));
}


/* Look for an address in a section.  This is called via
   bfd_map_over_sections.  */

void
Addr2Line::find_address_in_section (bfd *abfd, asection *section,
			 void *data)
{
  bfd_vma vma;
  bfd_size_type size;
  Addr2Line &t = *reinterpret_cast<Addr2Line*>(data);

  if (t.found)
    return;

  if ((bfd_get_section_flags (abfd, section) & SEC_ALLOC) == 0)
    return;

  vma = bfd_get_section_vma (abfd, section);
  if (t.pc < vma)
    return;

  size = bfd_get_section_size (section);
  if (t.pc >= vma + size)
    return;

  t.found = bfd_find_nearest_line (abfd, section, syms, t.pc - vma,
				 &t.filename, &t.functionname, &t.line);
}

/* Look for an offset in a section.  This is directly called.  */

void
Addr2Line::find_offset_in_section (asection *section)
{
  bfd_size_type size;

  if (found)
    return;

  if ((bfd_get_section_flags (abfd, section) & SEC_ALLOC) == 0)
    return;

  size = bfd_get_section_size (section);
  if (pc >= size)
    return;

  found = bfd_find_nearest_line (abfd, section, syms, pc,
				 &filename, &functionname, &line);
}


#endif

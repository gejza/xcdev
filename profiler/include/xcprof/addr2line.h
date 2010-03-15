/**
  * @author Todd Stock <toddastock@yahoo.com>
  * @date Feb 24  2002
  */
 
#ifndef ADDR2LINE
#define ADDR2LINE
 
#include <bfd.h>

/* 
 struct AddrInfo
 {
     char file_name[1024];
     char method_name[1024];
     long line;
     char hex[sizeof (void *) * 2 + 5];
 };
 
 class Addr2Line
 {
 
 public:
     Addr2Line (const char *);
     ~Addr2Line ();
 
     bool lookup(void *, AddrInfo* info);
 private:
  bfd *abfd;
  bfd_vma pc;
  const char *filename;
  const char *functionname;
  unsigned int line;
  bfd_boolean found;

static void
find_address_in_section (bfd *abfd, asection *section,
			 void *data);
void
slurp_symtab ();

void
find_offset_in_section (asection *section);
 };
*/

#endif


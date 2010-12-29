#ifdef __cplusplus
extern "C" {
#endif

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include "const-c.inc"
#ifdef __cplusplus
}
#endif

MODULE = XC::RD		PACKAGE = XC::RD

INCLUDE: const-xs.inc

void hello();
  CODE:
      printf("Tento text tiskne jazyk C\n");



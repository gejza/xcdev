#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <xc/error.h>

MODULE = XC		PACKAGE = XC

void
debug_enable()
	CODE:
		xc::debug::debug_enable();

void
log_stderr(mask)
		const char* mask
	CODE:
        xc::log::add_stderr(mask);



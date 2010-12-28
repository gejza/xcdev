/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifndef PHP_XCRD_H
#define PHP_XCRD_H

extern zend_module_entry xcrd_module_entry;
#define phpext_xcrd_ptr &xcrd_module_entry

#ifdef PHP_WIN32
#	define PHP_XCRD_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_XCRD_API __attribute__ ((visibility("default")))
#else
#	define PHP_XCRD_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_REGISTRY_VERSION "1.0"
#define PHP_REGISTRY_EXTNAME "registry"

PHP_MINIT_FUNCTION(xcrd);
PHP_MSHUTDOWN_FUNCTION(xcrd);
PHP_RINIT_FUNCTION(xcrd);
PHP_RSHUTDOWN_FUNCTION(xcrd);
PHP_MINFO_FUNCTION(xcrd);

PHP_FUNCTION(confirm_xcrd_compiled);	/* For testing, remove later. */

/**
 * Lookup include with php class defined
 */
PHP_FUNCTION(registry_php_class);

/**
 * Lookup template
 */
PHP_FUNCTION(xcr_template);

/**
 * Lookup string
 */
PHP_FUNCTION(registry_string);

//PHP_NAMED_FUNCTION(zif_memcache_pool_connect);
//PHP_NAMED_FUNCTION(zif_memcache_pool_addserver);

PHP_FUNCTION(registry_get);

PHP_FUNCTION(registry_set_lang);
PHP_FUNCTION(registry_add_ns);


/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(xcrd)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(xcrd)
*/

/* In every utility function you add that needs to use variables 
   in php_xcrd_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as XCRD_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define XCRD_G(v) TSRMG(xcrd_globals_id, zend_xcrd_globals *, v)
#else
#define XCRD_G(v) (xcrd_globals.v)
#endif

#endif	/* PHP_XCRD_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

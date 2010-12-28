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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_xcrd.h"

/* If you declare any globals in php_xcrd.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(xcrd)
*/

/* True global resources - no need for thread safety here */
static int le_xcrd;

/* {{{ xcrd_functions[]
 *
 * Every user visible function must have an entry in xcrd_functions[].
 */
const zend_function_entry xcrd_functions[] = {
	PHP_FE(confirm_xcrd_compiled,	NULL)		/* For testing, remove later. */
	{NULL, NULL, NULL}	/* Must be the last line in xcrd_functions[] */
};
/* }}} */

/* {{{ xcrd_module_entry
 */
zend_module_entry xcrd_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"xcrd",
	xcrd_functions,
	PHP_MINIT(xcrd),
	PHP_MSHUTDOWN(xcrd),
	PHP_RINIT(xcrd),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(xcrd),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(xcrd),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_XCRD
ZEND_GET_MODULE(xcrd)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("xcrd.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_xcrd_globals, xcrd_globals)
    STD_PHP_INI_ENTRY("xcrd.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_xcrd_globals, xcrd_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_xcrd_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_xcrd_init_globals(zend_xcrd_globals *xcrd_globals)
{
	xcrd_globals->global_value = 0;
	xcrd_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(xcrd)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(xcrd)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(xcrd)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(xcrd)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(xcrd)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "xcrd support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_xcrd_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_xcrd_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "xcrd", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

#if 0
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "php_registry.h"
#include "utils.h"

#include <stdint.h>
#include <xc/error.h>
#include <xc/cb.h>
#include <xc/serialize.h>
#include <xc/version.h>

#include <xc/registry/web.h>

static function_entry registry_functions[] = {
    PHP_FE(registry_php_class, NULL)

    PHP_FE(registry_string, NULL)
    PHP_FE(registry_get, NULL)
    PHP_FE(registry_add_ns, NULL)
    PHP_FE(registry_set_lang, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry registry_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_REGISTRY_EXTNAME,
    registry_functions,
    PHP_MINIT(registry),
    PHP_MSHUTDOWN(registry),
    NULL,
    NULL,
	PHP_MINFO(registry),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_REGISTRY_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_REGISTRY
ZEND_GET_MODULE(registry)
#endif

PHP_INI_BEGIN()
PHP_INI_ENTRY("registry.greeting", "Hello World", PHP_INI_ALL, NULL)
PHP_INI_END()

PHP_MINIT_FUNCTION(registry)
{
    REGISTER_INI_ENTRIES();

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(registry)
{
    UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}

PHP_MINFO_FUNCTION(registry)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "xc registry support", "enabled");
	php_info_print_table_row(2, "Version", xc::get_version());
	php_info_print_table_row(2, "Built", xc::get_built_info());
	php_info_print_table_row(2, "Info", xc::get_info());
	php_info_print_table_row(2, "Full info", xc::get_full_info());
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

PHP_FUNCTION(registry_php_class)
{
    try {
        char *name;
        int name_len;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
            RETURN_NULL();
        }

        php_printf("Search class %s \n", name);
        xc::registry::PHPIncludeTable_t table(*new xc::registry::Config_t());
        std::string file = table.find_class(name);
        if (!file.empty()) {
            RETURN_STRING(file.c_str(), 1);
        } else {
            RETURN_NULL();
        }
    } catch (const xc::error_t& e) {
        zend_error(E_ERROR, "Error: %s", e.message().c_str());
        RETURN_NULL();
    }
}

PHP_FUNCTION(registry_string)
{
    long id;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
        RETURN_NULL();
    }

    RETURN_STRING(INI_STR("registry.greeting"), 1);
    /*xc::CB_t cb;
    std::string str = cb.string(id, xc::CS);
    char* ret = estrdup(str.c_str());
    RETURN_STRING(ret, 0);*/
}


PHP_FUNCTION(registry_add_ns)
{
}

PHP_FUNCTION(registry_set_lang)
{
}

PHP_FUNCTION(registry_get)
{
    try {
        xc::CB_t registry;

        xc::serial_t out;
        xc::serial_t out2;
        xc::serial_t out3;
        //out.write("include", cb.include);
        //out.write("script", cb.script);
        out3.write("t1");
        out3.write("t1");
        out3.write("t1");
        out2.write("class", "test2");
        out2.write("sub", out3);
        out2.write("meth", "metoda");
        out.write("method", "test");
        out.write("obj2", out2);
        out.write("obj3", out3);

        xc::unserialize_t ser(out.str());
        load(return_value, ser);
        return;
    /*char *mystr;
    zval *mysubarray;
    array_init(return_value);
    //add_index_long(return_value, 42, 123);
    add_next_index_string(return_value, "I should now be found at index 43", 1);
    add_next_index_stringl(return_value, "I'm at 44!", 10, 1);
    
    mystr = estrdup(cb.string(1, xc::CS).c_str());
    add_next_index_string(return_value, mystr, 0);
    add_assoc_double(return_value, "pi", 3.1415926535);

    ALLOC_INIT_ZVAL(mysubarray);
    array_init(mysubarray);
    add_next_index_string(mysubarray, "registry", 1);
    add_next_index_string(mysubarray, "registry", 1);
    add_assoc_zval(return_value, "subarray", mysubarray);    
    //RETURN_STRING("Hello World!!!", 1);
    */
    } catch (const xc::error_t& e) {
        zend_error(E_ERROR, "Error: %s", e.message().c_str());
    }

}

#endif

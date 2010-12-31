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

#include <stdint.h>
#include <xc/error.h>
#include <xc/version.h>
#include <list>

#include <xc/rd/cdb.h>
//#include <xc/registry/web.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_xcrd.h"

#undef lookup
class Env_t
{
public:
	Env_t();
	~Env_t();
    
	void load(const char* fn);

	bool lookup(const xc::rd::ns_t ns, const xc::data_t& key,
			xc::data_t& value) const;

	void clear();
protected:
	typedef std::list<xc::rd::Lookup_t*> Tbs_t;
	Tbs_t _tbs;
};

ZEND_BEGIN_MODULE_GLOBALS(xcrd)
	Env_t env;
	xc::rd::Lookup_t* table;

	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(xcrd)

ZEND_DECLARE_MODULE_GLOBALS(xcrd)

/* True global resources - no need for thread safety here */
static int le_xcrd;

/* {{{ xcrd_functions[]
 *
 * Every user visible function must have an entry in xcrd_functions[].
 */
const zend_function_entry xcrd_functions[] = {
	PHP_FE(xcrd_load,	NULL)
	PHP_FE(xcrd_lookup,	NULL)		/* For testing, remove later. */
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
PHP_INI_BEGIN()
    //STD_PHP_INI_ENTRY("xcrd.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_xcrd_globals, xcrd_globals)
    //STD_PHP_INI_ENTRY("xcrd.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_xcrd_globals, xcrd_globals)
	PHP_INI_ENTRY("registry.greeting", "Hello World", PHP_INI_ALL, NULL)
PHP_INI_END()

/* }}} */

/* {{{ php_xcrd_init_globals
 */
static void php_xcrd_init_globals(zend_xcrd_globals *xcrd_globals)
{
	xcrd_globals->table = NULL;

	xcrd_globals->global_value = 0;
	xcrd_globals->global_string = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(xcrd)
{
	ZEND_INIT_MODULE_GLOBALS(xcrd, php_xcrd_init_globals, NULL);

	REGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(xcrd)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(xcrd)
{
	XCRD_G(global_string) = "rinit";
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
	php_info_print_table_row(2, "Version", xc::get_version());
	php_info_print_table_row(2, "Built", xc::get_built_info());
	php_info_print_table_row(2, "Info", xc::get_info());
	php_info_print_table_row(2, "Full info", xc::get_full_info());
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

PHP_FUNCTION(xcrd_load)
{
	try {
		char *fn = NULL;
		int fn_len;

		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &fn, &fn_len) == FAILURE) {
			return;
		}

		XCRD_G(env).load(fn);

    } catch (const xc::error_t& e) {
        zend_error(E_ERROR, "Error: %s", e.message().c_str());
        RETURN_NULL();
    }
}

Env_t::Env_t()
{
}
Env_t::~Env_t()
{
	this->clear();
}

void Env_t::load(const char* fn)
{
	/*if (XCRD_G(table))
		php_printf("Already loaded\n");

	XCRD_G(table) = new xc::rd::ConstDB_t(fn);*/
	xc::rd::Lookup_t* look = new xc::rd::ConstDB_t(fn);
	this->_tbs.push_front(look);
}

bool Env_t::lookup(const xc::rd::ns_t ns, const xc::data_t& key,
			xc::data_t& value) const
{
	for (Tbs_t::const_iterator l = this->_tbs.begin(); l != this->_tbs.end(); ++l)
	{
		if ((*l)->lookup(ns, key, value))
			return true;
	}
	return false;
}

PHP_FUNCTION(xcrd_lookup)
{
    try {
		long ns;
        const uint8_t* key;
        int key_len;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &ns, &key, &key_len) == FAILURE) {
            RETURN_NULL();
        }

		xc::data_t val;
		if (XCRD_G(env).lookup(ns, xc::data_t(key, key_len), val)) {
			RETURN_STRINGL((const char*)val.data(), val.size(), 1);
		} else {
			RETURN_NULL();
		}

    } catch (const xc::error_t& e) {
        zend_error(E_ERROR, "Error: %s", e.message().c_str());
        RETURN_NULL();
    }
}


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

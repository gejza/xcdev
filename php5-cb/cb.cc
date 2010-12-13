#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "php_cb.h"
#include "utils.h"

#include <stdint.h>
#include <xc/error.h>
#include <xc/cb.h>
#include <xc/serialize.h>
#include <xc/version.h>

static function_entry cb_functions[] = {
    PHP_FE(cb_get, NULL)
    PHP_FE(cb_string, NULL)
    PHP_FE(cb_add_ns, NULL)
    PHP_FE(cb_set_lang, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry cb_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_CB_EXTNAME,
    cb_functions,
    NULL,
    NULL,
    NULL,
    NULL,
	PHP_MINFO(cb),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_CB_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CB
ZEND_GET_MODULE(cb)
#endif

PHP_MINFO_FUNCTION(cb)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "cb support", "enabled");
	php_info_print_table_row(2, "Version", xc::get_version());
	php_info_print_table_row(2, "Built", xc::get_built_info());
	php_info_print_table_row(2, "Info", xc::get_info());
	php_info_print_table_row(2, "Full info", xc::get_full_info());
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

PHP_FUNCTION(cb_add_ns)
{
}

PHP_FUNCTION(cb_set_lang)
{
}

PHP_FUNCTION(cb_get)
{
    try {
        xc::CB_t cb;

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
    char *mystr;
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
    add_next_index_string(mysubarray, "hello", 1);
    add_next_index_string(mysubarray, "hello", 1);
    add_assoc_zval(return_value, "subarray", mysubarray);    
    //RETURN_STRING("Hello World!!!", 1);
    
    } catch (const xc::error_t& e) {
        zend_error(E_ERROR, "Error: %s", e.message().c_str());
    }

}

PHP_FUNCTION(cb_string)
{
    long id;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
        RETURN_NULL();
    }

    xc::CB_t cb;
    std::string str = cb.string(id, xc::CS);
    char* ret = estrdup(str.c_str());
    RETURN_STRING(ret, 0);
}


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_cb.h"

#include <stdint.h>
#include <xc/cb.h>

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
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_CB_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CB
ZEND_GET_MODULE(cb)
#endif

PHP_FUNCTION(cb_add_ns)
{
}

PHP_FUNCTION(cb_set_lang)
{
}

PHP_FUNCTION(cb_get)
{
    xc::CB_t cb;

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


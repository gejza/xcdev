#ifndef PHP_CB_H
#define PHP_CB_H 1

#define PHP_CB_VERSION "1.0"
#define PHP_CB_EXTNAME "cb"

//PHP_MINIT_FUNCTION(cb);
//PHP_MSHUTDOWN_FUNCTION(memcache);
PHP_MINFO_FUNCTION(cb);

//PHP_NAMED_FUNCTION(zif_memcache_pool_connect);
//PHP_NAMED_FUNCTION(zif_memcache_pool_addserver);

PHP_FUNCTION(cb_get);
PHP_FUNCTION(cb_string);

PHP_FUNCTION(cb_set_lang);
PHP_FUNCTION(cb_add_ns);

extern zend_module_entry cb_module_entry;
#define phpext_cb_ptr &cb_module_entry

#endif

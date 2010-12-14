#ifndef PHP_XC_REGISTRY_H
#define PHP_XC_REGISTRY_H 1

#define PHP_REGISTRY_VERSION "1.0"
#define PHP_REGISTRY_EXTNAME "registry"

PHP_MINIT_FUNCTION(registry);
PHP_MSHUTDOWN_FUNCTION(registry);
PHP_MINFO_FUNCTION(registry);

/**
 * Lookup include with php class defined
 */
PHP_FUNCTION(registry_php_class);

/**
 * Lookup string
 */
PHP_FUNCTION(registry_string);

//PHP_NAMED_FUNCTION(zif_memcache_pool_connect);
//PHP_NAMED_FUNCTION(zif_memcache_pool_addserver);



PHP_FUNCTION(registry_get);

PHP_FUNCTION(registry_set_lang);
PHP_FUNCTION(registry_add_ns);

extern zend_module_entry registry_module_entry;
#define phpext_registry_ptr &registry_module_entry

#endif

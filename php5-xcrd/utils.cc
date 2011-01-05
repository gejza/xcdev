#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>
#include <xc/error.h>

#include "php.h"
#include "utils.h"

//#include <xc/cb.h>

void xc::serialize(xc::buffer_t& out, const std::string& key, zval* val)
{
	switch (Z_TYPE_P(val)) {
        case IS_NULL:
            php_printf("NULL ");
            break;
        case IS_BOOL:
            php_printf("Boolean: %s ", Z_LVAL_P(val) ? "TRUE" : "FALSE");
            break;
        case IS_LONG:
			xc::serialize(out, key, Z_LVAL_P(val));
            break;
        case IS_DOUBLE:
			xc::serialize(out, key, Z_DVAL_P(val));
            break;
        case IS_STRING:
			xc::serialize(out, key, std::string(Z_STRVAL_P(val), Z_STRLEN_P(val)));
            /*php_printf("String: ");
            PHPWRITE(Z_STRVAL_P(val), Z_STRLEN_P(val));
            php_printf(" ");*/
            break;
        case IS_RESOURCE:
            php_printf("Resource ");
            break;
        case IS_ARRAY:
			xc::serialize(out, key, Z_ARRVAL_P(val));
            break;
        case IS_OBJECT:
            php_printf("Object ");
            break;
        default:
            php_printf("Unknown ");
	};
    php_printf("\n");
}

void xc::serialize(xc::buffer_t& out, const std::string& key, HashTable* arr)
{
	xc::serialize_t obj;
	int count = zend_hash_num_elements(arr);
	HashPosition pointer;
	zval** data;
	for(zend_hash_internal_pointer_reset_ex(arr, &pointer);
		zend_hash_get_current_data_ex(arr, (void**) &data, &pointer) == SUCCESS;
		zend_hash_move_forward_ex(arr, &pointer)) {

		char *key;
    	unsigned int key_len;
		unsigned long index;

		if (zend_hash_get_current_key_ex(arr, &key, &key_len, &index, 0, &pointer) == HASH_KEY_IS_STRING) {
			obj.add(std::string(key, key_len), *data);
    	} else {
			obj.add(*data);
    	}
    }
	/*obj.add("a", value.a);
	obj.add("b", value.b);
	obj.add("c", value.c);
	obj.add("n", value.n);*/
	serialize(out, key, obj);
}

void xc::load(zval* return_value, const xc::chunk_t& chunk)
{
	switch (chunk.id()) {
	case xc::chunk_t::ARRAY:
		{
			xc::unserialize_t sub(chunk);
			load(return_value, sub);
			break;
		}
		break;
	case xc::chunk_t::STRING:
		RETURN_STRINGL((char*)chunk.value(), chunk.value_size(), 1);
		break;
	case xc::chunk_t::LONG:
		RETURN_LONG(*((int*)chunk.value()));
		break;
	default:
		//zend_error(E_WARNING, "Unknown chunk id %x", (unsigned int)v->id());
		RUNTIME_ERROR("Unknown chunk id %x", (unsigned int)chunk.id());
		break;
	};
}

void xc::load(zval* return_value, xc::unserialize_t& ser)
{
    array_init(return_value);
    while (const xc::chunk_t* v = ser.read()) {

		zval* val;
		ALLOC_INIT_ZVAL(val);
		load(val, *v);
		if (v->key())
			add_assoc_zval(return_value, (char*)v->key(), val);
		else
			add_next_index_zval(return_value, val);
	}

}


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "utils.h"

#include <stdint.h>
#include <xc/error.h>

//#include <xc/cb.h>
#include <xc/serialize.h>
#include <xc/unserialize.h>

void load(zval* val, xc::unserialize_t& ser)
{
    array_init(val);
    while (xc::unserialize_t::value_t v = ser.read()) {
        switch (v.id()) {
        case xc::bytecode::ARRAY:
            {
                xc::unserialize_t sub = v.array();
                zval *mysubarray;
                ALLOC_INIT_ZVAL(mysubarray);
                array_init(mysubarray);
                load(mysubarray, sub);
                /*std::cout << "(" << std::endl;
                dump(sub, level + 1);
                std::cout << ")\n" << std::endl;*/
                if (v.key())
                    add_assoc_zval(val, (char*)v.key(), mysubarray);
                else
                    add_next_index_zval(val, mysubarray);
                break;
            }
            break;
        case xc::bytecode::STRING:
            if (v.key())
                add_assoc_string(val, (char*)v.key(), (char*)v.c_str(), 1);
            else
                add_next_index_string(val, (char*)v.c_str(), 1);
            break;
        default:
            zend_error(E_WARNING, "Unknown chunk id %x", (unsigned int)v.id());
            //RUNTIME_ERROR("Unknown chunk id %x", (unsigned int)v.id());
            break;
        };
    }
    return;
}


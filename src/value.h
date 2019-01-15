#ifndef __VALUE_H
#define __VALUE_H
#include "gc_object.h"
#include <stdint.h>

namespace interpretor{
    enum class value_type : uint8_t {
        NIL = 0,
        NUMBER,
        STRING,
        GC_OBJECT
    };
    class value{
    public:
        value_type      m_value_type;
        
        union{
            double      m_number;
            char*       m_string;
            gc_object*  m_gc_object;
        }data;

    public:
        value();
        value(value_type type, double val);
        value(value_type type, char* val);
        value(value_type type, gc_object* val);
    };
}

#endif
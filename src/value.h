#ifndef __VALUE_H
#define __VALUE_H
#include "gc_object.h"
#include <stdint.h>

namespace interpretor{
    enum class value_type : uint8_t {
        NIL = 0,
        INTEGER,
        DOUBLE,
        STRING,
        GC_OBJECT
    };

    class value{
    private:
        value_type      m_value_type;
        
        union{
            long        m_integer;
            double      m_float;
            char*       m_string;
            gc_object*  m_gc_object;
        }data;

    public:
        value(value_type type, long value) {
            m_value_type = type;
            data.m_integer = value;
        }
    };
}

#endif
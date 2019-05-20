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
            const char* m_string;
            gc_object*  m_gc_object;
        };

    public:
        value();
        value(double val);
        value(const char* val);
        value(gc_object* val);
        value operator+(value&);
        value operator-(value&);
        bool operator==(value&);
        value& operator=(value&);
        value& operator=(double);
        value& operator=(const char*);
        value& operator=(gc_object*);

        double number();
        const char* string();
        gc_object* gc_object();
        value_type type();
    };
}

#endif
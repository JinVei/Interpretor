#include "value.h"

interpretor::value::value(value_type type, double val) {
    m_value_type = type;
    data.m_number = val;
}

interpretor::value::value(value_type type, char * val)
{
    m_value_type  = type;
    data.m_string = val;
}

interpretor::value::value(value_type type, gc_object* val)
{
    m_value_type     = type;
    data.m_gc_object = val;
}

interpretor::value::value() {
    m_value_type = value_type::NIL;
    data.m_number = 0;
}


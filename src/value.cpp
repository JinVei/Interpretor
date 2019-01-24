#include "value.h"
#include "gc_object.h"
namespace interpretor {
    value::value() {
        m_value_type = value_type::NIL;
        m_number = 0;
    }
    value::value(double val) {
        this->m_value_type = value_type::NUMBER;
        this->m_number = val;
    }
    value::value(char * val) {
        m_value_type = value_type::STRING;
        m_string = val;
    }
    value::value(interpretor::gc_object* val) {
        m_value_type = value_type::GC_OBJECT;
        m_gc_object = val;
    }

    value value::operator+(value& val) {
        value ret_val = value();

        if (this->m_value_type == val.m_value_type) {
            switch (this->m_value_type) {
            case value_type::NUMBER:
                ret_val = value(this->number() + val.number());
                break;
            case value_type::GC_OBJECT:
                //ret_val = value(*(this->gc_object()) + *(val.gc_object()));
                break;
            case value_type::STRING:
                break;
            case value_type::NIL:
                break;
            default:
                break;
            }
        }         
        return ret_val;
    }

    value value::operator-(value& val) {
        value ret_val = value();

        if (this->m_value_type == val.m_value_type) {
            switch (this->type()) {
            case value_type::NUMBER:
                ret_val = value(this->number() - val.number());
                break;
            case value_type::GC_OBJECT:
                break;
            case value_type::STRING:
                break;
            case value_type::NIL:
                break;
            default:
                break;
            }
        }
        return ret_val;
    }

    bool value::operator==(value& val) {
        bool ret = false;

        if (this->type() == val.type()) {
            switch (this->type()) {
            case value_type::NUMBER:
                ret = (this->number() == val.number() ? true : false);
                break;
            case value_type::GC_OBJECT:
                ret = (this->gc_object() == val.gc_object() ? true : false);
                break;
            case value_type::STRING:
                ret = (std::string(this->string()) == std::string(val.string()) ? true : false);
                break;
            case value_type::NIL:
                ret = true;
                break;
            default:
                break;
            }
        }
        return ret;
    }

    double value::number() {
        return m_number;
    }

    char * value::string() {
        return m_string;
    }

    gc_object* value::gc_object() {
        return m_gc_object;
    }

    value_type value::type() {
        return m_value_type;
    }
}

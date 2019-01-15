#include "table_type.h"

namespace interpretor {
    table::table() {}

    auto table::create_table() -> gc_object *
    {
        return new table();
    }

    auto table::mark_gc_flag() -> void {
        set_gc_recycle_flag();
        for (auto& object : m_table) {
            if (object.second.m_value_type == value_type::GC_OBJECT) {
                object.second.data.m_gc_object->mark_gc_flag();
            }
        }
    }

    value table::get_value(const char * name) {
        auto& it_value = m_table.find(name);
        if (it_value != m_table.end()) {
            return (*it_value).second;
        }
        else {
            return value(value_type::NIL, 0);
        }
    }
    void table::put_value(const char * name, value val) {
            m_table[name] = val;
        }
}

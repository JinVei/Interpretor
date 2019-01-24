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
                gc_object::mark_gc_flag(object.second.m_gc_object);
            }
        }
    }

    value table::get_value(const char * name) {
        auto& it_value = m_table.find(std::string(name));
        if (it_value != m_table.end()) {
            return (*it_value).second;
        }
        else {
            return value();
        }
    }
    void table::put_value(const char * name, value val) {
            m_table[std::string(name)] = val;
        }
}

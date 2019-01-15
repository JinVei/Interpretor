#ifndef __TABLE_TYPE_H
#define __TABLE_TYPE_H
#include "gc_object.h"
#include "value.h"

#include <map>

namespace interpretor {
    class table : public gc_object{
    private:
        std::map<const char*, value> m_table;
    public:
        table();
        static auto create_table()->gc_object*;
        auto mark_gc_flag() -> void override;
        value get_value(const char* name);
        void put_value(const char* name, value val);
    };
}
#endif
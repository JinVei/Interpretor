#include "gc_object_type.h"
#include "gc_object.h"
#include "table_type.h"

#include <functional>

namespace interpretor {
    static gc_object* test_empty() {
        return nullptr;
    }
    gc_object_type_item gc_object_type_table[] = {
        { gc_object_type::table, "table", table::create_table },
    };
}

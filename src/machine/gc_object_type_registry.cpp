#include "gc_object_type_registry.h"
#include "gc_object.h"
#include "table_type.h"

#include <functional>
#include <unordered_map>

namespace interpretor {
    std::unordered_map<gc_object_type, gc_object_type_item> gc_object_type_registry = {
        { gc_object_type::TABLE,
          gc_object_type_item{ gc_object_type::TABLE, "table", table::create_table },
        },

    };
}

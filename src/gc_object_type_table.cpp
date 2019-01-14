#include "gc_object_type.h"
#include "gc_object.h"

#include <functional>

namespace interpretor {
    static gc_object* test_empty() {
        return nullptr;
    }
    gc_object_type_item gc_object_type_table[] = {
        { gc_object_type::cons, "test", test_empty },
    };
}

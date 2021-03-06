#ifndef __GC_OBJECT_TYPE_H
#define __GC_OBJECT_TYPE_H
#include <stdint.h>
#include <functional>
#include <unordered_map>

namespace interpretor {
    class gc_object;
    using gc_object_creator = std::function<gc_object*() >;
    using gc_object_type_size = uint16_t;
    enum class gc_object_type : gc_object_type_size;

    struct gc_object_type_item {
        gc_object_type      _type;
        const char*         _label;
        gc_object_creator   _creator;
    };

    enum class gc_object_type : gc_object_type_size {
        TABLE = 0,
    };

    extern std::unordered_map<gc_object_type, gc_object_type_item> gc_object_type_registry;
}

#endif
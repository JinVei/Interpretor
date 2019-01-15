#ifndef __GC_OBJECT_TYPE_H
#define __GC_OBJECT_TYPE_H
#include <stdint.h>
#include <functional>
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

    enum class gc_object_type : gc_object_type_size {//using std::arrry
        table = 0,
    };

    extern gc_object_type_item gc_object_type_table[];
}

#endif
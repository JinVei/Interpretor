#ifndef __GARBAGE_COLLECTOR_H
#define __GARBAGE_COLLECTOR_H
#include "gc_object_type_registry.h"
#include "gc_object.h"
#include "value.h"

#include <memory>
#include <list>

namespace interpretor {
    class garbage_collector {
    private:
        std::list<std::shared_ptr<gc_object>> m_gc_object_list;
    public:
        value create_gc_object(gc_object_type type);
        void garbage_recycle_mark(gc_object* root);
        void garbage_recycle_clean();
    };

    extern garbage_collector g_garbage_collector;
}
#endif
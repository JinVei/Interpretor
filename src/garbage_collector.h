#ifndef __GARBAGE_COLLECTOR_H
#define __GARBAGE_COLLECTOR_H
#include "gc_object_type.h"
#include "gc_object.h"

#include <memory>
#include <list>

namespace interpretor {
    class garbage_collector {
    private:
        std::list<std::shared_ptr<gc_object>> m_gc_object_list;
    public:
        auto create_gc_object(gc_object_type_size type) -> std::weak_ptr<gc_object>;
        void garbage_recycle_mark(gc_object* root);
        void garbage_recycle_clean();
    };
}
#endif
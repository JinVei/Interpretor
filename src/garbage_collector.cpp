#include "garbage_collector.h"

namespace interpretor {

    auto garbage_collector::create_gc_object(gc_object_type_size type) -> std::weak_ptr<gc_object> {
        gc_object* object = gc_object_type_table[type]._creator();

        std::shared_ptr<gc_object> ptr_object(object);
        m_gc_object_list.push_back(ptr_object);

        return ptr_object;
    }

    void garbage_collector::garbage_recycle_mark(gc_object * root) {
        root->mark_gc_flag();
    }

    void garbage_collector::garbage_recycle_clean() {
        for (auto it = m_gc_object_list.begin(); it != m_gc_object_list.end(); ) {
            if ( (*it)->m_gc_recycle_flag == true) {
                (*it)->m_gc_recycle_flag = false;
                it++;
            } else {
                it = m_gc_object_list.erase(it);
            }
        }
    }

}

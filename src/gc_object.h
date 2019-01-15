#ifndef __GC_OBJECT_H
#define __GC_OBJECT_H
#include "gc_object_type.h"

namespace interpretor{
    class gc_object{
        friend class garbage_collector;
    private:
        bool                m_gc_recycle_flag = false;
    protected:
        gc_object_type      m_type_tag;

        void set_gc_recycle_flag() {
            m_gc_recycle_flag = true;
        }
    public:
        //derived class must override this method to recycle its gc_object members(by invoking mark_gc_flag())
        virtual void mark_gc_flag() {
            set_gc_recycle_flag();
        }
    };
}

#endif
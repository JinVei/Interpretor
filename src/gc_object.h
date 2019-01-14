#ifndef __GC_OBJECT_H
#define __GC_OBJECT_H

namespace interpretor{
    class gc_object{
        friend class garbage_collector;
    private:
        bool m_gc_recycle_flag = false;
    protected:
        void set_gc_flag() {
            m_gc_recycle_flag = true;
        }
        virtual void mark_gc_flag() {
            set_gc_flag();
        }
    };
}

#endif
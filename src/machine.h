#ifndef __MACHINE_H
#define __MACHINE_H

#include "stack.h"
#include "instruction.h"

#include <vector>

namespace interpretor{
    class logger {
        public: virtual void log(char* message) {}
    };

    class machine{
    private:
        value                       m_nil_val;
        value                       m_register_ret_index;
        value                       m_register_pc;
        value                       m_register_stack_frame_button_index;
        //value                       m_register_stack_top_index;

        int                         m_error_flag = 0;
        int                         m_run_flag   = 0;
        std::vector<value>          m_stack;
        std::vector<instruction>    m_repertoire;

        logger                      m_printer;

        void init_register();
    public:
        machine();
        machine(logger printer);
        void run();
        void evaluate(instruction instruction);
        void execute_instruction(instruction instruction);
        void increase_pc();
        void set_pc(value new_pc);
        void print(char* message);
        void set_run_error();
        void set_returned_value(value val);

        void stack_push(value val);
        value stack_pop();
        value stack_top();
        value& stack_index(value index);
    };
}
#endif
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
        value                       m_register_ret;
        value                       m_register_pc;

        int                         m_error_flag = 0;
        stack                       m_stack;
        std::vector<instruction>    m_instruction_table;

        logger                      printer;
    public:
        void run();
        void evaluate(instruction instruction);
        void increase_pc();
        void print(char* message);
        void set_run_error();
        void set_returned_value(value val);
    };
}
#endif
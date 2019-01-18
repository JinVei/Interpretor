#ifndef __MACHINE_H
#define __MACHINE_H

#include "stack.h"
#include "instruction.h"
#include "plog.h"
#include <vector>

namespace interpretor{
#define MACHINE_PRINT_LOG(mac, log) (mac).print(PLOG_FUNCTION_LOCATION_INFO " " log)
    class logger {
        public: virtual void log(const char* message) {}
    };

    class machine{
    private:
        unsigned int                m_register_ret_index;
        unsigned int                m_register_pc;
        unsigned int                m_register_flag;
        unsigned int                m_register_ebp_index;
        unsigned int                m_stack_guard_index;

        int                         m_error_flag = 0;
        int                         m_run_flag   = 0;
        
        std::vector<value>          m_stack;
        std::vector<instruction>    m_repertoire;

        value                       m_nil_val;

        logger*                      m_printer;

        void init_register();
    public:
        machine();
        machine(logger* printer);
        void run();
        void execute_instruction(instruction instruction);
        void increase_pc();
        void print(char* message);

        void set_run_error();
        void set_pc(unsigned int new_pc);
        void set_returned_reg(value val);
        void set_flag_reg(unsigned int flag);
        void set_repertoire(std::vector<instruction>& repertoire);

        value get_returned_reg();
        unsigned int get_flag_reg();

        void stack_push(value val);
        value stack_pop();
        value stack_top();
        value& stack_index(value index);
        unsigned int stack_length();
    };
}
#endif
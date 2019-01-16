#include "machine.h"
#include <thread>

namespace interpretor {
    void machine::init_register() {
        //value register_pc = value(value_type::NUMBER, 0.0);
        //stack_push(register_pc);
        //m_register_pc_index = stack_top();

        value register_ret = value();
        stack_push(register_ret);
        m_register_ret_index = stack_top();

        value stack_guard = value();
        stack_push(stack_guard);
        m_register_stack_frame_button_index = stack_top();
    }

    machine::machine() {
        init_register();
    }

    machine::machine(logger printer) {
        init_register();
        m_printer = printer;
    }

    void machine::run() {
        m_run_flag = 1;
        unsigned int pc = 0;
        while (m_run_flag) {
            if (m_register_pc.m_value_type != value_type::NUMBER) {
                print("if (m_register_pc.m_value_type != value_type::NUMBER)");
                set_run_error();
            }
            pc = (unsigned int)m_register_pc.data.m_number;
            if (pc < 0 || pc >= m_repertoire.size()) {
                print("if (m_register_pc.m_value_type != value_type::NUMBER)");
                set_run_error();
            }
            execute_instruction(m_repertoire[pc]);
        }
    }
    void machine::execute_instruction(instruction instruction) {
        std::list<value> operator_param;
        operand operand_val;
        value   tmp_val;
        while (instruction._operands_list.size() > 0) {
            operand_val = instruction._operands_list.front();

            switch (operand_val._operand_type) {
              case operand_type::immediate_operand :
                  operator_param.push_back(operand_val._val);
                  break;
              case operand_type::stack_operand :
                  tmp_val = stack_index(operand_val._val);
                  operator_param.push_back(tmp_val);
                  break;
            }

            instruction._operands_list.pop_front();
        }
        operator_registry[instruction._operator_type]._operator_handler(*this, operator_param);
    }

    void machine::increase_pc() {
        ++(m_register_pc.data.m_number);
    }

    void machine::set_pc(value new_pc) {
        m_register_pc = new_pc;
    }

    void machine::print(char* message) {
        m_printer.log(message);
    }

    void machine::set_run_error() {//need modify
        m_error_flag = 1;
        while (m_error_flag != 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            print("loop in set_run_error");
        }
        return;
    }

    void machine::set_returned_value(value val) {
        value& rer_val = stack_index(m_register_ret_index);
        rer_val = val;
    }

    void machine::stack_push(value val) {
        m_stack.push_back(val);
    }

    value machine::stack_pop() {
        value top = m_stack.back();
        m_stack.pop_back();
        return top;
    }

    value machine::stack_top() {
        return value(value_type::NUMBER, m_stack.size() - 1);
    }

    value& machine::stack_index(value index) {
        if (index.m_value_type != value_type::NUMBER
            || index.data.m_number < 0
            || index.data.m_number > stack_top().data.m_number) {

            print("stack over index");
            set_run_error();
            return m_nil_val;
        }
        return m_stack[(unsigned int)index.data.m_number];
    }
}
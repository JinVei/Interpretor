#include "machine.h"
#include <thread>

namespace interpretor {
    void machine::init_register() {
        m_register_pc = 0;
        m_register_flag = 0;

        value register_ret = value();
        stack_push(register_ret);
        m_register_ret_index = stack_length();

        value stack_guard = value();
        stack_push(stack_guard);
        m_register_stack_frame_button_index = stack_length();
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
        while (m_run_flag) {
            m_register_pc;
            if (m_register_pc < 0 || m_register_pc >= m_repertoire.size()) {
                print("m_register_pc < 0 || m_register_pc >= m_repertoire.size()");
                set_run_error();
            }
            execute_instruction(m_repertoire[m_register_pc]);
        }
    }
    void machine::execute_instruction(instruction instruction) {
        std::list<value> operator_param;
        operand operand_val;
        while (instruction._operands_list.size() > 0) {
            operand_val = instruction._operands_list.front();
            value stack_val;

            switch (operand_val._operand_type) {
              case operand_type::immediate_operand :
                  operator_param.push_back(operand_val._val);
                  break;
              case operand_type::stack_operand :
                  stack_val = stack_index(operand_val._val);
                  operator_param.push_back(stack_val);
                  break;
              case operand_type::register_operand:
                  if(operand_val._val == value(0.0))
                      operator_param.push_back(value(m_register_pc));
                  else if(operand_val._val == value(1))
                      operator_param.push_back(stack_index(m_register_ret_index));
                  else {
                      operator_param.push_back(value());
                      print("operand_type::register_operand is unrecognizable");
                      set_run_error();
                  }
                  break;
              default:
                  print("not case to operand_val._operand_type");
                  break;
            }

            instruction._operands_list.pop_front();
        }
        operator_registry[instruction._operator_type]._operator_handler(*this, operator_param);
    }

    void machine::increase_pc() {
        ++m_register_pc;
    }

    void machine::set_pc(unsigned int new_pc) {

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

    void machine::set_returned_reg(value val) {
        value& rer_val = stack_index(m_register_ret_index);
        rer_val = val;
    }

    void machine::set_flag_reg(unsigned int flag) {
        m_register_flag = flag;
    }

    value machine::get_returned_reg() {
        return stack_index(m_register_ret_index);
    }

    unsigned int machine::get_flag_reg() {
        return m_register_flag;
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
        return value(m_stack.size() - 1);
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
    unsigned int machine::stack_length() {
        return m_stack.size() - 1;
    }
}
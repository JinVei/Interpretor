#include "plog.h"
#include "machine.h"
#include <ctime>
#include <thread>

namespace interpretor {
    void machine::init_register() {
        m_register_pc = 0;
        m_register_flag = 0;

        value register_ret = value();
        stack_push(register_ret);
        m_register_ret_index = stack_length();

        value ebp = value(stack_length() + 1);
        stack_push(ebp);
        m_register_ebp_index = stack_length();

        value stack_guard;
        stack_push(stack_guard);
        m_stack_guard_index = stack_length();

        stack_index(value(m_register_ebp_index)) = stack_length();
    }

    machine::machine() {
        init_register();
        m_printer = nullptr;
    }

    machine::machine(logger* printer) {
        init_register();
        m_printer = printer;
    }

    void machine::run() {
        m_run_flag = 1;
        while (m_run_flag) {
            if (m_register_pc < 0 || m_register_pc >= m_repertoire.size()) {
                //print("m_register_pc < 0 || m_register_pc >= m_repertoire.size()");
                MACHINE_PRINT_LOG(*this, "\nm_register_pc < 0 || m_register_pc >= m_repertoire.size()");
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
            value offset = operand_val._offset;
            if (offset == operand_register_type::ebp_reg_type())
                offset = stack_index(m_register_ebp_index);
            else if (offset == operand_register_type::ret_reg_type())
                offset = stack_index(m_register_ret_index);
            else
                offset = value(0.0);

            switch (operand_val._operand_type) {
              case operand_type::immediate_operand :
                  operator_param.push_back(operand_val._val);
                  break;
              case operand_type::stack_operand :
                  stack_val = stack_index(operand_val._val + offset);
                  operator_param.push_back(stack_val);
                  break;
              case operand_type::register_operand:
                  if(operand_val._val == operand_register_type::pc_reg_type())
                      operator_param.push_back(value(m_register_pc));
                  else if(operand_val._val == operand_register_type::ret_reg_type())
                      operator_param.push_back(stack_index(m_register_ret_index));
                  else if (operand_val._val == operand_register_type::ebp_reg_type()) {
                      //value ebp = stack_index(value(m_register_ebp_index));
                      //operator_param.push_back(stack_index( ebp + operand_val._offset));
                      operator_param.push_back(stack_index(m_register_ebp_index));
                  } else {
                      operator_param.push_back(value());
                      MACHINE_PRINT_LOG(*this, "\n""operand_type::operand_register_type is unrecognizable");
                      set_run_error();
                  }
                  break;
              case operand_type::register_address_operand:
                  if (operand_val._val == operand_register_type::ret_reg_type())
                      operator_param.push_back(m_register_ret_index);
                  else if (operand_val._val == operand_register_type::ebp_reg_type())
                      operator_param.push_back(m_register_ebp_index);
                  else {
                      operator_param.push_back(value());
                      MACHINE_PRINT_LOG(*this, "\n""operand_type::operand_register_type is unrecognizable");
                      set_run_error();
                  }
                  break;
              case operand_type::stack_index_operand:
                  //if (operand_val._val == operand_register_type::ret_reg_type())
                  //    operator_param.push_back(value(m_register_ret_index) + operand_val._offset);
                  //else if (operand_val._val == operand_register_type::ebp_reg_type())
                  //    operator_param.push_back(value(m_register_ebp_index) + operand_val._offset);
                  //else {
                  //    operator_param.push_back(value());
                  //    MACHINE_PRINT_LOG(*this, "\n""operand_type::operand_register_type is unrecognizable");
                  //    set_run_error();
                  //}
                  //stack_val = stack_index(operand_val._val + offset);
                  operator_param.push_back(operand_val._val + offset);
                  break;
              default:
                  MACHINE_PRINT_LOG(*this, "\n""not case to operand_val._operand_type");
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

    void machine::print(char* log) {
        if (m_printer != nullptr) {
//            std::chrono::system_clock::time_point now_time = std::chrono::system_clock::now();
//            std::time_t time = std::chrono::system_clock::to_time_t(now_time);
            char time[64]; 
            GetTime(time, 64);
            m_printer->log((std::string(time) + " " + std::string(log) + "\n").c_str());
        }
    }

    void machine::set_run_error() {//need modify
        m_error_flag = 1;
        MACHINE_PRINT_LOG(*this, "\nloop in set_run_error");
        while (m_error_flag != 0) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
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

    void machine::set_repertoire(std::vector<instruction>& repertoire) {
        m_repertoire = repertoire;
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
        if (stack_length() == m_stack_guard_index)
            return value();

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
            || index.data.m_number > stack_top().data.m_number) 
        {
            MACHINE_PRINT_LOG(*this, "\n" "Index.m_value_type != value_type::NUMBER or Stack over index");
            set_run_error();
            return m_nil_val;
        }
        return m_stack[(unsigned int)index.data.m_number];
    }
    unsigned int machine::stack_length() {
        return m_stack.size() - 1;
    }
}
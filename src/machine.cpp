#include "plog.h"
#include "machine.h"
#include <ctime>
#include <thread>

namespace interpretor {

    void machine::init_register() {
        m_nil_val = value();
        m_register_pc = 0;
        m_register_flag = 0;

        register_init_list reg_init;
        for (uint_t i = 0; i < reg_init.list.size(); ++i) {
            auto it = m_registers_index.find(reg_init.list[i]);
            if (it  == m_registers_index.end()) {
                stack_push(value(0.0));
                m_registers_index[reg_init.list[i]] = stack_top();
            } else{
                MACHINE_PRINT_LOG((*this),"m_registers_index.find(reg_init_list[i]) != end()");
            }
        }

        value stack_guard;
        stack_push(stack_guard);
        m_stack_guard_index = (uint_t)stack_top().number();

        value index = m_registers_index[register_t::ebp];
        stack_index(index) = stack_top().number();
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
            //value index;

            if ((offset.type() == value_type::NUMBER)
             && (m_registers_index.find(value_to_register(offset)) != m_registers_index.end())) {
                value& index = m_registers_index[value_to_register(offset)];
                offset = stack_index(index);
            } else {
                offset = value(0.0);
            }

            switch (operand_val._operand_type) {
              case operand_type::immediate_operand :
                  operator_param.push_back(operand_val._val);
                  break;
              case operand_type::stack_operand :
                  stack_val = stack_index(operand_val._val + offset);
                  operator_param.push_back(stack_val);
                  break;
              case operand_type::register_operand:
/*                  if(operand_val._val == register_type::pc_reg_type())
                      operator_param.push_back(value(m_register_pc));
                  else */
                  if (m_registers_index.find(value_to_register(operand_val._val)) != m_registers_index.end()) {
                      value& index = m_registers_index[value_to_register(operand_val._val)];
                      stack_val = stack_index(index);
                      operator_param.push_back(stack_val);
                  } else {
                          operator_param.push_back(value());
                          MACHINE_PRINT_LOG(*this, "\n""operand_type::operand_register_type is unrecognizable");
                          set_run_error();
                  }
                  break;
              case operand_type::register_address_operand:
                  if (m_registers_index.find(value_to_register(operand_val._val)) != m_registers_index.end()) {
                      value& index = m_registers_index[value_to_register(operand_val._val)];
                      operator_param.push_back(index);
                  } else {
                      operator_param.push_back(value());
                      MACHINE_PRINT_LOG(*this, "\n""operand_type::operand_register_type is unrecognizable");
                      set_run_error();
                  }
                  break;
              case operand_type::stack_index_operand:
                  if (operand_val._val.type() != value_type::NUMBER) {
                      MACHINE_PRINT_LOG(*this, "\n""if (operand_val._val.type() != value_type::NUMBER)");
                      set_run_error();
                  }
                  operator_param.push_back(operand_val._val + offset);
                  break;
              default:
                  MACHINE_PRINT_LOG(*this, "\n""not case to operand_val._operand_type");
                  set_run_error();
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
            char time[64]; 
            GetTime(time, 64);
            m_printer->log((std::string(time) + " " + std::string(log) + "\n").c_str());
        }
    }

    void machine::garbage_collection() {
        for (auto& val : m_stack) {
            if (val.m_value_type == value_type::GC_OBJECT) {
                m_garbage_collector.garbage_recycle_mark( val.gc_object() );
            }
        }
    }

    auto machine::new_gc_object(gc_object_type type) -> value{
        value val = m_garbage_collector.create_gc_object(type);
        return val;
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
        value& rer_val = stack_index(m_registers_index[register_t::ret]);
        rer_val = val;
    }

    void machine::set_flag_reg(unsigned int flag) {
        m_register_flag = flag;
    }

    void machine::set_repertoire(std::vector<instruction>& repertoire) {
        m_repertoire = repertoire;
    }

    value machine::get_returned_reg() {
        return stack_index(m_registers_index[register_t::ret]);
    }

    unsigned int machine::get_flag_reg() {
        return m_register_flag;
    }

    void machine::stack_push(value val) {
        m_stack.push_back(val);
    }

    value machine::stack_pop() {
        if (stack_top().number() == m_stack_guard_index) {
            MACHINE_PRINT_LOG(*this, "\n" "stack_top().number() == m_stack_guard_index");
            set_run_error();
            return m_nil_val;
        }

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
        return m_stack[(uint_t)index.data.m_number];
    }
}
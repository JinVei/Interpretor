#include "plog.h"
#include "machine.h"
#include "operator_registry.h"

#include <ctime>
#include <thread>

namespace interpretor {

    void machine::init_register() {
        m_nil_val = value();
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

        m_register_pc_index = (unsigned int) m_registers_index[register_t::pc].number();

        value index = m_registers_index[register_t::ebp];
        value& ebp = stack_index(index);
        ebp = stack_top().number();
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
            value& pc = stack_index(m_register_pc_index);

            if (pc.number() < 0 || pc.number() >= m_repertoire.size()) {
                MACHINE_PRINT_LOG(*this, "\nm_register_pc < 0 || m_register_pc >= m_repertoire.size()");
                set_run_error();
            }
            execute_instruction(m_repertoire[(unsigned int)pc.number()]);
        }
    }
    void machine::shut_down() {
        m_run_flag = 0;
    }
    void machine::execute_instruction(instruction instruction) {
        operator_registry[instruction._operator_type]._operator_handler(*this, instruction._operands_list);
    }

    value& machine::check_oprand(operand& operand_val, unsigned int mask) {

        switch (operand_val._operand_type) {
        case operand_type::immediate_operand:
        {
            value offset = operand_val._offset;
            
            value reg_val;
            if (operand_val._offset.type() == value_type::NUMBER) {
                auto find_it = m_registers_index.find(value_to_register(offset));
                if (find_it == m_registers_index.end())
                    offset = value(0.0);
                else {
                    value index = find_it->second;
                    reg_val = stack_index(index);
                }
            }
            if (reg_val == value())
                return std::move(operand_val._val);
            else
                return std::move(operand_val._val + reg_val);
        }
        case operand_type::stack_index_operand:
        case operand_type::stack_operand:
        {
            value offset = operand_val._offset;
            value index;
            value reg_val;
            if (operand_val._offset.type() == value_type::NUMBER) {
                auto find_it = m_registers_index.find(value_to_register(offset));
                if (find_it == m_registers_index.end())
                    offset = value(0.0);
                else {
                    index = find_it->second;
                    reg_val = stack_index(index);
                }
            }
            else {
                offset = value(0.0);
            }

            return stack_index(operand_val._val + reg_val);
        }
        case operand_type::register_address_operand:
        case operand_type::register_operand:
        {
            auto reg_index_it = m_registers_index.find(value_to_register(operand_val._val));
            if (reg_index_it != m_registers_index.end()) {
                value& index = reg_index_it->second;

                return stack_index(index);

            }
            else {
                MACHINE_PRINT_LOG(*this, "\n""operand_type::operand_register_type is unrecognizable");
                set_run_error();
                return m_nil_val;

            }
        }
        
        default:
            MACHINE_PRINT_LOG(*this, "\n""not case to operand_val._operand_type");
            set_run_error();
            return m_nil_val;
        }
    }

    void machine::increase_pc() {
        value& pc = stack_index(m_register_pc_index);
        ++(pc.m_number);
    }

    void machine::set_pc(unsigned int new_pc) {
        value& pc = stack_index(m_register_pc_index);
        pc.m_number = new_pc;
    }

    void machine::print(const char* log) {
        char time[64];
        GetTime(time, 64);
        m_log_message = (std::string(time) + " " + std::string(log) + "\n");
        if (m_printer != nullptr) {
            m_printer->log(m_log_message.c_str());
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
        //add error_callbake here
        //#ifdef DEBUG
        MACHINE_PRINT_LOG(*this, "\nloop in set_run_error");
        while (m_error_flag != 0) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        //#endif //DEBUG
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
            || index.number() < 0
            || index.number() > stack_top().number()) 
        {
            MACHINE_PRINT_LOG(*this, "\n" "Index.m_value_type != value_type::NUMBER or Stack over index");
            set_run_error();
            return m_nil_val;
        }
        return m_stack[(uint_t)index.number()];
    }

}
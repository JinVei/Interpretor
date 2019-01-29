#include "plog.h"
#include "primative_operator.h"
#include "garbage_collector.h"
#include "table_type.h"

#include <array>
namespace interpretor {
    void primative_operator::empty_operation(machine& machine, std::list<value>& operands) {
        machine.increase_pc();
    }
    void primative_operator::addtion(machine& machine, std::list<value>& operands) {
        value operand1;
        value operand2;
        double number;

        if (operands.size() < 2) {
            MACHINE_PRINT_LOG(machine, "\n""the number of operands must not less than 2");
            goto Error1;
        }

        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        operands.pop_front();

        if (operand1.m_value_type != value_type::NUMBER
            || operand2.m_value_type != value_type::NUMBER) {
            MACHINE_PRINT_LOG(machine, "\n""operands type must be value_type::NUMBER");
            goto Error1;
        }

        number = operand1.number() + operand2.number();
        machine.set_returned_reg(value(number));
        machine.increase_pc();
        return;
    Error1:
        machine.set_returned_reg(value());
        machine.set_run_error();
    }

    void primative_operator::subtraction(machine & machine, std::list<value>& operands) {
        value operand1;
        value operand2;
        double number;

        if (operands.size() < 2) {
            MACHINE_PRINT_LOG(machine, "\n""the number of operands must not less than 2");
            goto Error1;
        }

        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        operands.pop_front();

        if (operand1.m_value_type != value_type::NUMBER
            || operand2.m_value_type != value_type::NUMBER) {
            MACHINE_PRINT_LOG(machine, "\n""operands type must be value_type::NUMBER");
            goto Error1;
        }

        number = operand1.number() - operand2.number();
        machine.set_returned_reg(value(number));
        machine.increase_pc();
        return;
    Error1:
        machine.set_returned_reg(value());
        machine.set_run_error();
    }

    void primative_operator::multiplication(machine & machine, std::list<value>& operands) {
        value operand1;
        value operand2;
        double number;

        if (operands.size() < 2) {
            MACHINE_PRINT_LOG(machine, "\n""the number of operands must not less than 2");
            goto Error1;
        }

        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        operands.pop_front();

        if (operand1.m_value_type != value_type::NUMBER
            || operand2.m_value_type != value_type::NUMBER) {
            MACHINE_PRINT_LOG(machine, "\n""operands type must be value_type::NUMBER");
            goto Error1;
        }

        number = operand1.number() * operand2.number();
        machine.set_returned_reg(value(number));
        machine.increase_pc();
        return;
    Error1:
        machine.set_returned_reg(value());
        machine.set_run_error();
    }

    void primative_operator::division(machine & machine, std::list<value>& operands) {
        value operand1;
        value operand2;
        double number;

        if (operands.size() < 2) {
            MACHINE_PRINT_LOG(machine, "\n""the number of operands must not less than 2");
            goto Error1;
        }

        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        operands.pop_front();

        if (operand1.m_value_type != value_type::NUMBER
            || operand2.m_value_type != value_type::NUMBER) {
            MACHINE_PRINT_LOG(machine, "\n""operands type must be value_type::NUMBER");
            goto Error1;
        }
        if (fabs(operand2.number()) < std::numeric_limits<double>::epsilon()) {
            MACHINE_PRINT_LOG(machine, "\n""divisor type must cannot be zero");
            goto Error1;
        }

        number = operand1.number() / operand2.number();
        machine.set_returned_reg(value(number));
        machine.increase_pc();
        return;
    Error1:
        machine.set_returned_reg(value());
        machine.set_run_error();
    }

    void primative_operator::jump(machine & machine, std::list<value>& operands) {
        value operand1;

        if (operands.size() < 1) {
            MACHINE_PRINT_LOG(machine, "\n""the number of operands must not less than 1");
            goto Error1;
        }

        operand1 = operands.front();
        operands.pop_front();

        if (operand1.m_value_type != value_type::NUMBER) {
            MACHINE_PRINT_LOG(machine, "\n""operands type must be value_type::NUMBER");
            goto Error1;
        }

        machine.set_pc((unsigned int)operand1.number());
        return;
    Error1:
        machine.set_run_error();
    }

    void primative_operator::push(machine & machine, std::list<value>& operands) {
        value operand1;

        if (operands.size() < 1) {
            MACHINE_PRINT_LOG(machine, "\n""the number of operands must not less than 1");
            goto Error1;
        }
        operand1 = operands.front();
        operands.pop_front();

        machine.stack_push(operand1);

        machine.increase_pc();
        return;
    Error1:
        machine.set_run_error();
    }

    void primative_operator::pop(machine & machine, std::list<value>& operands) {
        value ret_val = machine.stack_pop();
        machine.set_returned_reg(ret_val);
        machine.increase_pc();
    }

    void primative_operator::compare(machine & machine, std::list<value>& operands) {
        value operand1;
        value operand2;

        if (operands.size() < 2) {
            MACHINE_PRINT_LOG(machine, "\n""the number of operands must not less than 2");
            goto Error1;
        }
        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        if (operand1 == operand2) {
            machine.set_flag_reg(1);
        } else {
            machine.set_flag_reg(0);
        }

        machine.increase_pc();
        return;
    Error1:
        machine.set_run_error();
    }

    void primative_operator::branch(machine & machine, std::list<value>& operands) {
        value operand1;

        if (operands.size() < 1) {
            MACHINE_PRINT_LOG(machine, "\n""the number of operands must not less than 1");
            goto Error1;
        }
        if (machine.get_flag_reg() != 1) {
            machine.increase_pc();
            goto Exit1;
        }

        operand1 = operands.front();
        if (operand1.type() != value_type::NUMBER) {
            MACHINE_PRINT_LOG(machine, "\n""operand1.type != value_type::NUMBER");
            goto Error1;
        }
        machine.set_pc((unsigned int)operand1.number());

    Exit1:
        return;
    Error1:
        machine.set_run_error();
    }

    void primative_operator::move(machine & machine, std::list<value>& operands) {
        value operand1;
        value operand2;
        if (operands.size() < 2) {
            MACHINE_PRINT_LOG(machine, "\n""the number of operands must not less than 2");
            goto Error1;
        }
        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        if (operand1.type() != value_type::NUMBER) {
            MACHINE_PRINT_LOG(machine, "\n""if (operand1.type() != value_type::NUMBER)");
            goto Error1;
        }
        machine.stack_index(operand1) = operand2;

        machine.increase_pc();;
        return;
    Error1:
        machine.set_run_error();
    }

    void primative_operator::new_table(machine & machine, std::list<value>& operands) {
        value val = machine.new_gc_object(gc_object_type::TABLE);
        table* table_val = dynamic_cast<table*>(val.m_gc_object);
        value key_name;

        if (table_val == nullptr) {
            MACHINE_PRINT_LOG(machine, "\n""table_val == nullptr");
            goto Error1;
        }
        while (2 <= operands.size()) {
            key_name = operands.front();
            if (key_name.m_value_type != value_type::STRING) {
                MACHINE_PRINT_LOG(machine, "\n""name must be string type");
                goto Error1;
            }
            operands.pop_front();

            val = operands.front();
            operands.pop_front();

            table_val->put_value(key_name.m_string, val);
        }
        machine.set_returned_reg(value(table_val));
        machine.increase_pc();;
        return;
    Error1:
        machine.set_returned_reg(value());
        machine.set_run_error();
    }

    void primative_operator::table_get(machine & machine, std::list<value>& operands){
        value table_val;
        value key_name;
        value ret_val;
        table* table1;

        if (operands.size() < 2) {
            MACHINE_PRINT_LOG(machine, "\n""operands.size() < 2");
            goto Error1;
        }

        table_val = operands.front();
        operands.pop_front();
        key_name = operands.front();

        if (table_val.m_value_type != value_type::GC_OBJECT) {
            MACHINE_PRINT_LOG(machine, "\n""table_val.m_value_type != value_type::GC_OBJECT");
            goto Error1;
        }
        if (key_name.m_value_type != value_type::STRING) {
            MACHINE_PRINT_LOG(machine, "\n""key_name.m_value_type != value_type::STRING");
            goto Error1;
        }

        table1 = dynamic_cast<table*>(table_val.m_gc_object);
        if (table1 == nullptr) {
            MACHINE_PRINT_LOG(machine, "\n""dynamic_cast<table*>(table_val.m_gc_object) == nullptr");
            goto Error1;
        }
        ret_val = table1->get_value(key_name.m_string);

        machine.set_returned_reg(ret_val);
        machine.increase_pc();
        return;

    Error1:
        machine.set_returned_reg(value());
        machine.set_run_error();
    }

    void primative_operator::table_put(machine & machine, std::list<value>& operands) {
        value table_val;
        value key_name;
        value put_val;
        table* table1;

        if (operands.size() < 3) {
            MACHINE_PRINT_LOG(machine, "\n""operands.size() < 3");
            goto Error1;
        }

        table_val = operands.front();
        operands.pop_front();
        key_name = operands.front();
        operands.pop_front();
        put_val = operands.front();

        if (table_val.m_value_type != value_type::GC_OBJECT) {
            MACHINE_PRINT_LOG(machine, "\n""table_val.m_value_type != value_type::GC_OBJECT");
            goto Error1;
        }
        if (key_name.m_value_type != value_type::STRING) {
            MACHINE_PRINT_LOG(machine, "\n""key_name.m_value_type != value_type::STRING");
            goto Error1;
        }

        table1 = dynamic_cast<table*>(table_val.m_gc_object);
        if (table1 == nullptr) {
            MACHINE_PRINT_LOG(machine, "\n""dynamic_cast<table*>(table_val.m_gc_object) == nullptr");
            goto Error1;
        }
        table1->put_value(key_name.m_string, put_val);

        machine.set_returned_reg(value());
        machine.increase_pc();
        return;
    Error1:
        machine.set_returned_reg(value());
        machine.set_run_error();
    }

    void primative_operator::log(machine& machine, std::list<value>& operands) {
        if (operands.size() < 0) {
            MACHINE_PRINT_LOG(machine, "\n""operands.size() < 0\n");
            return;
        }
        if (operands.front().type() != value_type::STRING) {
            MACHINE_PRINT_LOG(machine, "\n""operands.front().type != value_type::STRING\n");
            return;
        }
        machine.print(operands.front().string());
    }
    void primative_operator::shut_down(machine& machine, std::list<value>& operands) {
        machine.shut_down();
    }
}
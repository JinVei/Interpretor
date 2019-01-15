#include "primative_operator.h"
#include "garbage_collector.h"
#include "table_type.h"

#include <array>
namespace interpretor {
    void primative_operator::empty_operator(machine& machine, std::list<value> operands) {
        machine.increase_pc();
    }
    void primative_operator::addtion(machine& machine, std::list<value> operands) {
        value operand1;
        value operand2;
        double number;

        if (operands.size() < 2) {
            machine.print("the number of operands must not less than 2");
            goto Exit1;
        }

        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        operands.pop_front();

        if (operand1.m_value_type != value_type::NUMBER
            || operand2.m_value_type != value_type::NUMBER) {
            machine.print("operands type must be value_type::NUMBER");
            goto Exit1;
        }

        number = operand1.data.m_number + operand2.data.m_number;
        machine.set_returned_value(value(value_type::NUMBER, number));
        machine.increase_pc();
        return;
    Exit1:
        machine.set_returned_value(value());
        machine.set_run_error();
        //machine.increase_pc();
    }

    void primative_operator::subtraction(machine & machine, std::list<value> operands) {
        value operand1;
        value operand2;
        double number;

        if (operands.size() < 2) {
            machine.print("the number of operands must not less than 2");
            goto Exit1;
        }

        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        operands.pop_front();

        if (operand1.m_value_type != value_type::NUMBER
            || operand2.m_value_type != value_type::NUMBER) {
            machine.print("operands type must be value_type::NUMBER");
            goto Exit1;
        }

        number = operand1.data.m_number - operand2.data.m_number;
        machine.set_returned_value(value(value_type::NUMBER, number));
        machine.increase_pc();
        return;
    Exit1:
        machine.set_returned_value(value());
        machine.set_run_error();
    }

    void primative_operator::multiplication(machine & machine, std::list<value> operands) {
        value operand1;
        value operand2;
        double number;

        if (operands.size() < 2) {
            machine.print("the number of operands must not less than 2");
            goto Exit1;
        }

        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        operands.pop_front();

        if (operand1.m_value_type != value_type::NUMBER
            || operand2.m_value_type != value_type::NUMBER) {
            machine.print("operands type must be value_type::NUMBER");
            goto Exit1;
        }

        number = operand1.data.m_number * operand2.data.m_number;
        machine.set_returned_value(value(value_type::NUMBER, number));
        machine.increase_pc();
        return;
    Exit1:
        machine.set_returned_value(value());
        machine.set_run_error();
    }

    void primative_operator::division(machine & machine, std::list<value> operands) {
        value operand1;
        value operand2;
        double number;

        if (operands.size() < 2) {
            machine.print("the number of operands must not less than 2");
            goto Exit1;
        }

        operand1 = operands.front();
        operands.pop_front();
        operand2 = operands.front();
        operands.pop_front();

        if (operand1.m_value_type != value_type::NUMBER
            || operand2.m_value_type != value_type::NUMBER) {
            machine.print("operands type must be value_type::NUMBER");
            goto Exit1;
        }
        if (fabs(operand2.data.m_number) < std::numeric_limits<double>::epsilon()) {
            machine.print("divisor type must cannot be zero");
            goto Exit1;
        }

        number = operand1.data.m_number / operand2.data.m_number;
        machine.set_returned_value(value(value_type::NUMBER, number));
        machine.increase_pc();
        return;
    Exit1:
        machine.set_returned_value(value());
        machine.set_run_error();
    }

    void primative_operator::new_table(machine & machine, std::list<value> operands) {
        value val = g_garbage_collector.create_gc_object(gc_object_type::TABLE);
        table* table_val = dynamic_cast<table*>(val.data.m_gc_object);
        value key_name;

        if (table_val == nullptr) {
            machine.print("table_val == nullptr");
            goto Exit1;
        }
        while (2 <= operands.size()) {
            key_name = operands.front();
            if (key_name.m_value_type != value_type::STRING) {
                machine.print("name must be string type");
                goto Exit1;
            }
            operands.pop_front();

            val = operands.front();
            operands.pop_front();

            table_val->put_value(key_name.data.m_string, val);
        }
        machine.set_returned_value(value(value_type::GC_OBJECT, table_val));
        machine.increase_pc();;
        return;
    Exit1:
        machine.set_returned_value(value());
        machine.set_run_error();
    }

    void primative_operator::table_get(machine & machine, std::list<value> operands){
        value table_val;
        value key_name;
        value ret_val;
        table* table1;

        if (operands.size() < 2) {
            machine.print("operands.size() < 2");
            goto Exit1;
        }

        table_val = operands.front();
        operands.pop_front();
        key_name = operands.front();

        if (table_val.m_value_type != value_type::GC_OBJECT) {
            machine.print("table_val.m_value_type != value_type::GC_OBJECT");
            goto Exit1;
        }
        if (key_name.m_value_type != value_type::STRING) {
            machine.print("key_name.m_value_type != value_type::STRING");
            goto Exit1;
        }

        table1 = dynamic_cast<table*>(table_val.data.m_gc_object);
        if (table1 == nullptr) {
            machine.print("dynamic_cast<table*>(table_val.data.m_gc_object) == nullptr");
            goto Exit1;
        }
        ret_val = table1->get_value(key_name.data.m_string);

        machine.set_returned_value(ret_val);
        machine.increase_pc();
        return;

    Exit1:
        machine.set_returned_value(value());
        machine.set_run_error();
    }

    void primative_operator::table_put(machine & machine, std::list<value> operands) {
        value table_val;
        value key_name;
        value put_val;
        table* table1;

        if (operands.size() < 3) {
            machine.print("operands.size() < 3");
            goto Exit1;
        }

        table_val = operands.front();
        operands.pop_front();
        key_name = operands.front();
        operands.pop_front();
        put_val = operands.front();

        if (table_val.m_value_type != value_type::GC_OBJECT) {
            machine.print("table_val.m_value_type != value_type::GC_OBJECT");
            goto Exit1;
        }
        if (key_name.m_value_type != value_type::STRING) {
            machine.print("key_name.m_value_type != value_type::STRING");
            goto Exit1;
        }

        table1 = dynamic_cast<table*>(table_val.data.m_gc_object);
        if (table1 == nullptr) {
            machine.print("dynamic_cast<table*>(table_val.data.m_gc_object) == nullptr");
            goto Exit1;
        }
        table1->put_value(key_name.data.m_string, put_val);

        machine.set_returned_value(value());
        machine.increase_pc();
        return;
    Exit1:
        machine.set_returned_value(value());
        machine.set_run_error();
    }
}
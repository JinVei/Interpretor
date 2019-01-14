#include "primative_operator.h"

void interpretor::empty_operator (interpretor::machine& machine, std::list<interpretor::value> operands) {
    machine.increase_pc();
}
void interpretor::addtion (interpretor::machine& machine, std::list<interpretor::value> operands) {
    if (operands.size() < 2) {
        machine.print("the number of operands must not less than 2");
        machine.set_run_error();
        machine.set_ret_register(value(value_type::NIL, 0));
        return;
    }

    value operand1 = operands.front();
    operands.pop_front();
    value operand2 = operands.front();
    operands.pop_front();

    if (operand1.m_value_type != value_type::NUMBER
        || operand2.m_value_type != value_type::NUMBER) {
        machine.print("operands type must be value_type::NUMBER");
        machine.set_run_error();
        machine.set_ret_register(value(value_type::NIL, 0));
        return;
    }

    double number = operand1.data.m_number + operand2.data.m_number;
    machine.set_ret_register(value(value_type::NUMBER, number));
}

void interpretor::subtraction(interpretor::machine & machine, std::list<interpretor::value> operands) {
    if (operands.size() < 2) {
        machine.print("the number of operands must not less than 2");
        machine.set_run_error();
        machine.set_ret_register(value(value_type::NIL, 0));
        return;
    }

    value operand1 = operands.front();
    operands.pop_front();
    value operand2 = operands.front();
    operands.pop_front();

    if (operand1.m_value_type != value_type::NUMBER
        || operand2.m_value_type != value_type::NUMBER) {
        machine.print("operands type must be value_type::NUMBER");
        machine.set_run_error();
        machine.set_ret_register(value(value_type::NIL, 0));
        return;
    }

    double number = operand1.data.m_number - operand2.data.m_number;
    machine.set_ret_register(value(value_type::NUMBER, number));
}

void interpretor::multiplication(interpretor::machine & machine, std::list<interpretor::value> operands) {
    if (operands.size() < 2) {
        machine.print("the number of operands must not less than 2");
        machine.set_run_error();
        machine.set_ret_register(value(value_type::NIL, 0));
        return;
    }

    value operand1 = operands.front();
    operands.pop_front();
    value operand2 = operands.front();
    operands.pop_front();

    if (operand1.m_value_type != value_type::NUMBER
        || operand2.m_value_type != value_type::NUMBER) {
        machine.print("operands type must be value_type::NUMBER");
        machine.set_run_error();
        machine.set_ret_register(value(value_type::NIL, 0));
        return;
    }

    double number = operand1.data.m_number * operand2.data.m_number;
    machine.set_ret_register(value(value_type::NUMBER, number));
}

void interpretor::division(interpretor::machine & machine, std::list<interpretor::value> operands) {
    if (operands.size() < 2) {
        machine.print("the number of operands must not less than 2");
        machine.set_run_error();
        machine.set_ret_register(value(value_type::NIL, 0));
        return;
    }

    value operand1 = operands.front();
    operands.pop_front();
    value operand2 = operands.front();
    operands.pop_front();

    if (operand1.m_value_type != value_type::NUMBER
        || operand2.m_value_type != value_type::NUMBER) {
        machine.print("operands type must be value_type::NUMBER");
        machine.set_run_error();
        machine.set_ret_register(value(value_type::NIL, 0));
        return;
    }
    if (fabs(operand2.data.m_number) < std::numeric_limits<double>::epsilon()) {
        machine.print("divisor type must cannot be zero");
        machine.set_run_error();
        machine.set_ret_register(value(value_type::NIL, 0));
        return;
    }
    double number = operand1.data.m_number / operand2.data.m_number;
    machine.set_ret_register(value(value_type::NUMBER, number));
}

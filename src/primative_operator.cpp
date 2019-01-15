#include "primative_operator.h"

void interpretor::empty_operator (interpretor::machine& machine, std::list<interpretor::value> operands) {
    machine.increase_pc();
}
void interpretor::addtion (interpretor::machine& machine, std::list<interpretor::value> operands) {
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
    machine.set_returned_value(value(value_type::NIL, 0));
    machine.set_run_error();
    //machine.increase_pc();
}

void interpretor::subtraction(interpretor::machine & machine, std::list<interpretor::value> operands) {
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
    machine.set_returned_value(value(value_type::NIL, 0));
    machine.set_run_error();
}

void interpretor::multiplication(interpretor::machine & machine, std::list<interpretor::value> operands) {
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
    machine.set_returned_value(value(value_type::NIL, 0));
    machine.set_run_error();
}

void interpretor::division(interpretor::machine & machine, std::list<interpretor::value> operands) {
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
    machine.set_returned_value(value(value_type::NIL, 0));
    machine.set_run_error();
}

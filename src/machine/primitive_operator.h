#ifndef __primitive_OPERATOR_H
#define __primitive_OPERATOR_H
#include "machine.h"

namespace interpretor{
    struct primitive_operator {
        static void empty_operation(machine& machine, operand_list operands);
        static void addtion(machine& machine, operand_list operands);
        static void subtraction(machine& machine, operand_list operands);
        static void multiplication(machine& machine, operand_list operands);
        static void division(machine& machine, operand_list operands);
        static void jump(machine& machine, operand_list operands);
        static void push(machine& machine, operand_list operands);
        static void pop(machine& machine, operand_list operands);
        static void compare(machine& machine, operand_list operands);
        static void branch(machine& machine, operand_list operands);
        static void move(machine& machine, operand_list operands);
        static void new_table(machine& machine, operand_list operands);
        static void table_get(machine& machine, operand_list operands);
        static void table_put(machine& machine, operand_list operands);
        static void log(machine& machine, operand_list operands);
        static void shut_down(machine& machine, operand_list operands);
    };
}
#endif

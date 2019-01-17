#ifndef __PRIMATIVE_OPERATOR_H
#define __PRIMATIVE_OPERATOR_H
#include "machine.h"

namespace interpretor{
    struct primative_operator {
        static void empty_operation(machine& machine, std::list<value>& operands);
        static void addtion(machine& machine, std::list<value>& operands);
        static void subtraction(machine& machine, std::list<value>& operands);
        static void multiplication(machine& machine, std::list<value>& operands);
        static void division(machine& machine, std::list<value>& operands);
        static void jump(machine& machine, std::list<value>& operands);
        static void push(machine& machine, std::list<value>& operands);
        static void pop(machine& machine, std::list<value>& operands);
        static void compare(machine& machine, std::list<value>& operands);
        static void branch(machine& machine, std::list<value>& operands);
        static void move(machine& machine, std::list<value>& operands);
        static void new_table(machine& machine, std::list<value>& operands);
        static void table_get(machine& machine, std::list<value>& operands);
        static void table_put(machine& machine, std::list<value>& operands);
    };
}
#endif

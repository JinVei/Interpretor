#ifndef __PRIMATIVE_OPERATOR_H
#define __PRIMATIVE_OPERATOR_H
#include "machine.h"

namespace interpretor{
    struct primative_operator {
        static void empty_operator(machine& machine, std::list<value> operands);
        static void addtion(machine& machine, std::list<value> operands);
        static void subtraction(machine& machine, std::list<value> operands);
        static void multiplication(machine& machine, std::list<value> operands);
        static void division(machine& machine, std::list<value> operands);
        static void new_table(machine& machine, std::list<value> operands);
        static void table_get(machine& machine, std::list<value> operands);
        static void table_put(machine& machine, std::list<value> operands);
    };
}
#endif

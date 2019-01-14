#include "operator_type.h"
#include "operation.h"
#include "machine.h"
#include "primative_operator.h"

using namespace interpretor;


operation oprations_table[] = {
    operation(operator_type::EMPTY,             "empty",    empty_operator),
    operation(operator_type::ADDITION,          "add",      addtion),
    operation(operator_type::SUBTRACTION,       "sub",      subtraction),
    operation(operator_type::MULTIPLICATION,    "mul",      multiplication),
    operation(operator_type::DIVISION,          "div",      division),
};
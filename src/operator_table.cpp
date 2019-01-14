#include "operator_type.h"
#include "operation.h"
#include "machine.h"
#include "primative_operator.h"

namespace interpretor {
    operator_item operator_table[] = {
        {operator_type::EMPTY,             "empty",    empty_operator},
        {operator_type::ADDITION,          "add",      addtion},
        {operator_type::SUBTRACTION,       "sub",      subtraction},
        {operator_type::MULTIPLICATION,    "mul",      multiplication},
        {operator_type::DIVISION,          "div",      division},
    };
}

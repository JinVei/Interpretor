#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H
#include "operator_registry.h"
#include "value.h"

#include <list>
namespace interpretor{
    enum class operand_type : uint8_t {
        immediate_operand = 0,
        stack_operand,
//        register_operand,
    };

    struct operand {
        operand_type _operand_type;
        value        _val;
    };

    struct instruction{
        operator_type         _operator_type;
        std::list<operand>    _operands_list;
    };
}
#endif
#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H
#include "operator_registry.h"
#include "value.h"

#include <stdint.h>
#include <list>
namespace interpretor{
    enum class operand_type : uint8_t {
        immediate_operand = 0,
        stack_operand,
        register_operand,//0->pc_reg, 1->ret_reg, 2->ebp_reg
        register_address_operand,
        stack_index_operand,
    };
    using register_size = uint8_t;
    enum class register_t : register_size {
        pc,
        ret,
        ebp,
    };
    extern value register_to_value(register_t reg);
    extern register_t value_to_register(value val);

    struct register_init_list {
         std::vector<register_t> list = {
             register_t::pc,
             register_t::ret,
             register_t::ebp
         };
    };
    struct operand {
        operand_type _operand_type;
        value        _val;
        value        _offset;
        operand();
        operand(operand_type type, value val);
        operand(operand_type type, value val, value offset);
    };

    struct instruction{
        operator_type         _operator_type;
        std::list<operand>    _operands_list;
    };
}
#endif
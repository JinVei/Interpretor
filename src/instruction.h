#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H
#include "operator_registry.h"
#include "value.h"

#include <list>
namespace interpretor{
    enum class operand_type : uint8_t {
        immediate_operand = 0,
        stack_operand,
        register_operand,//0->pc_reg, 1->ret_reg, 2->ebp_reg
        register_address_operand,
        stack_index_operand,
    };
    struct register_t{
         //static value pc_reg_type() {
         //    return value(0.0);
         //}
         static value ret() {
             return value(1);
         }
         static value ebp() {
             return value(2);
         }
    };
    struct register_init_list {
         std::vector<value> list = {
             //register_t::pc(),
             register_t::ret(),
             register_t::ebp()
         };
    };
    struct operand {
        operand_type _operand_type;
        value        _val;
        value        _offset;
        operand() {
            _offset = value();
        }
        operand(operand_type type, value val) {
            _operand_type = type;
            _val          = val;
            _offset       = value();
        }
        operand(operand_type type, value val, value offset) {
            _operand_type = type;
            _val = val;
            if(offset.type() == value_type::NUMBER)
                _offset = offset;
            else
                _offset = value();
        }
    };

    struct instruction{
        operator_type         _operator_type;
        std::list<operand>    _operands_list;
    };
}
#endif
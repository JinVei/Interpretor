#include "instruction.h"
namespace interpretor {
    value register_to_value(register_t reg) {
        return value(static_cast<register_size>(reg));
    }
    register_t value_to_register(value val) {
        return static_cast<register_t>((register_size)val.number());
    }
    operand::operand() {
        _offset = value();
    }
    operand::operand(operand_type type, value val) {
        _operand_type = type;
        _val = val;
        _offset = value();
    }
    operand::operand(operand_type type, value val, value offset) {
        _operand_type = type;
        _val = val;
        if (offset.type() == value_type::NUMBER)
            _offset = offset;
        else
            _offset = value();
    }
}
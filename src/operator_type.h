#ifndef __OPERATOR_TYPE_H
#define __OPERATOR_TYPE_H
#include <stdint.h>
#include <functional>
#include <list>

namespace interpretor {
    class value;
    class machine;
    using operator_handle = std::function<void(machine& machine, std::list<value>)>;
    using operator_type_size = uint16_t;
    enum class operator_type : operator_type_size;

    struct operator_item {
        operator_type       _operator_type;
        const char*         _instruction_label;
        operator_handle     _operator_handler;
    };

    enum class operator_type : operator_type_size {
        EMPTY = 0,
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        GOTO,
    };

    extern operator_item operator_table[];
}
#endif

#ifndef __OPERATOR_TYPE_H
#define __OPERATOR_TYPE_H
#include <stdint.h>
#include <functional>
#include <list>
#include <unordered_map>

namespace interpretor {
    class value;
    class machine;
    using operator_handle = std::function<void(machine&, std::list<value>& )>;
    using operator_type_size = uint16_t;
    enum class operator_type : operator_type_size;

    struct operator_item {
        operator_type       _operator_type;
        const char*         _instruction_label;
        operator_handle     _operator_handler;
        const char*         _decription;
    };

    enum class operator_type : operator_type_size {
        EMPTY = 0,
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        JUMP,
        PUSH,
        POP,
        CMP,
        BRANCH,
        TABLE,
        TABLE_GET,
        TABLE_PUT,
        //GOTO,
    };

    extern std::unordered_map<operator_type, operator_item> operator_registry;
}
#endif

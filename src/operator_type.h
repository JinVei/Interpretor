#ifndef __OPERATOR_TYPE_H
#define __OPERATOR_TYPE_H
#include <stdint.h>

namespace interpretor {
    struct operation;

    enum class operator_type : uint16_t {
        EMPTY = 0,
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        GOTO,
    };

    extern operation opration_table[];
}
#endif

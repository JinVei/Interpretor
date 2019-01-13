#ifndef __OPERATION_H
#define __OPERATION_H
#include "value.h"

#include <string>
#include <list>
#include <functional>

namespace interpretor {
    using operation_handle = std::function<std::list<value>(std::list<value>)>;
    enum class operation_type : uint16_t;
    struct operation {
        operation_type      type;
        std::string         label;
        operation_handle    handler;

        operation(operation_type type, const char* label, operation_handle handler) {
            this->type      = type;
            this->label     = std::string(label);
            this->handler   = handler;
        }
    };

    enum class operation_type : uint16_t {
        EMPTY = 0,
        ADDITION,
        SUBTRACTION,
        DIVISION,
        MULTIPLICATION,
    };

    extern operation opration_table[];
}
#endif
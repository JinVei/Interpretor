#ifndef __OPERATION_H
#define __OPERATION_H
#include "value.h"

#include <string>
#include <list>
#include <functional>

namespace interpretor {
    class machine;
    enum class operator_type : uint16_t;

    using operation_handle = std::function<void(machine& machine, std::list<value>)>;

    struct operation {
        operator_type       _operator_type;
        std::string         _instruction_label;
        operation_handle    _operator_handler;

        operation(operator_type type, const char* label, operation_handle handler);
    };
}
#endif
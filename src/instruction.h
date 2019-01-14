#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H
#include "operation.h"
#include "value.h"

#include <list>
namespace interpretor{
    struct instruction{
        operator_type       operator_type;
        std::list<value>    operands_list;
    };
}
#endif
#ifndef __MACHINE_H
#define __MACHINE_H

#include "stack.h"
#include "instruction.h"

#include <vector>

namespace interpretor{
    class machine{
    private:
        stack                       m_stack;
        std::vector<instruction>    m_instruction_table;
    public:
        void run();
        void evaluate(instruction instruction);
    };
}
#endif
#ifndef __PRIMATIVE_OPERATOR_H
#define __PRIMATIVE_OPERATOR_H
#include "machine.h"

namespace interpretor{
    void empty_operator (interpretor::machine& machine, std::list<interpretor::value> operands);
    void addtion (interpretor::machine& machine, std::list<interpretor::value> operands);
    void subtraction (interpretor::machine& machine, std::list<interpretor::value> operands);
    void multiplication (interpretor::machine& machine, std::list<interpretor::value> operands);
    void division (interpretor::machine& machine, std::list<interpretor::value> operands);
}
#endif

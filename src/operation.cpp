#include "operation.h"
#include "machine.h"
#include "primative_operator.h"

interpretor::operation::operation(operator_type type, const char * label, operation_handle handler) {
    this->_operator_type = type;
    this->_instruction_label = std::string(label);
    this->_operator_handler = handler;
}

#include "operation.h"
using namespace interpretor;

auto empty(std::list<value> opreands) -> std::list<value> {
    std::list<value> return_list;
    //return_list.push_back(value(value_type::NIL, 0));
    return return_list;
}

operation oprations_table[] = {
    operation(operation_type::EMPTY, "empty", empty )
};

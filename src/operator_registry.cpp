#include "operator_registry.h"
#include "primative_operator.h"
#include <unordered_map>

namespace interpretor {
    std::unordered_map<operator_type, operator_item> operator_registry = {
        { operator_type::EMPTY, 
          operator_item{ operator_type::EMPTY,             "empty",        primative_operator::empty_operation }
        },
        { operator_type::ADDITION,
          operator_item{ operator_type::ADDITION,          "add",          primative_operator::addtion,
                        "add [number1] [number2]\n"
                        "The resunlt will be stored in ret_reg\n"}
        },
        { operator_type::SUBTRACTION,
          operator_item{ operator_type::SUBTRACTION,       "sub",          primative_operator::subtraction,
                        "sub [number1] [number2]\n"
                        "The resunlt will be stored in ret_reg\n" }
        },
        { operator_type::MULTIPLICATION,
          operator_item{ operator_type::MULTIPLICATION,    "mul",          primative_operator::multiplication,
                        "MUL [number1] [number2]\n"
                        "The resunlt will be stored in ret_reg\n" }
        },
        { operator_type::DIVISION,
          operator_item{ operator_type::DIVISION,          "div",          primative_operator::division,
                        "div [number1] [number2]\n"
                        "The resunlt will be stored in ret_reg\n" }
        },
        { operator_type::JUMP,
          operator_item{ operator_type::JUMP,              "jmp",          primative_operator::jump,
                        "jmp [address]\n"
                        "Jump to the specified address\n"}
        },
        { operator_type::PUSH,
          operator_item{ operator_type::PUSH,              "push",         primative_operator::push,
                        "push [value]\n"\
                        "Push one value in stack\n" }
        },
        { operator_type::POP,
          operator_item{ operator_type::POP,               "pop",         primative_operator::pop,
                        "pop []\n"\
                        "Pop a value from stack. The result will be stored in ret_reg.\n" }
        },
        { operator_type::CMP,
          operator_item{ operator_type::CMP,               "cmp",         primative_operator::compare,
                        "cmp [value1] [value2]\n"\
                        "Compare the value of [operand1] and [operand2]. will update flag_reg\n" }
        },
        { operator_type::BRANCH,
          operator_item{ operator_type::BRANCH,            "branch",      primative_operator::branch,
                        "branch [address]\n"\
                        "If the flag_ret has been flagged, will jump to specified address, or continue to next address\n" }
        },
        { operator_type::MOV,
          operator_item{ operator_type::MOV,               "mov",         primative_operator::move,
                        "cmp [index2] [index2]\n"\
                        "Move the value of index2 into index1 \n" }
        },
        { operator_type::TABLE,
          operator_item{ operator_type::TABLE,             "table",        primative_operator::new_table, 
                        "table [value1] [value2] [value3] ...\n"
                        "Create a table type. The result will be stored in ret_reg.\n"}
        },
        { operator_type::TABLE_GET,
          operator_item{ operator_type::TABLE_GET,         "table_get",    primative_operator::table_get,
                        "table_get [table] [key_name]\n"
                        "Get a matched value from table. The result will be stored in ret_reg.\n"}
        },
        { operator_type::TABLE_PUT,
          operator_item{ operator_type::TABLE_PUT,         "table_put",    primative_operator::table_put,
                        "table_put [table] [key_name] [value1]\n"
                        "Put a key-value pair into table"}
        },
    };
}

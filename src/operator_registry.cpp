#include "operator_registry.h"
#include "primative_operator.h"
#include <unordered_map>

namespace interpretor {
    std::unordered_map<operator_type, operator_item> operator_registry = {
        { operator_type::EMPTY, 
          operator_item{ operator_type::EMPTY,             "empty",        primative_operator::empty_operator }
        },
        { operator_type::ADDITION,
          operator_item{ operator_type::ADDITION,          "add",          primative_operator::addtion }
        },
        { operator_type::SUBTRACTION,
          operator_item{ operator_type::SUBTRACTION,       "sub",          primative_operator::subtraction }
        },
        { operator_type::MULTIPLICATION,
          operator_item{ operator_type::MULTIPLICATION,    "mul",          primative_operator::multiplication } 
        },
        { operator_type::DIVISION,
          operator_item{ operator_type::DIVISION,          "div",          primative_operator::division }
        },
        { operator_type::TABLE,
          operator_item{ operator_type::TABLE,             "table",        primative_operator::new_table, 
                        "create table" }
        },
        { operator_type::TABLE_GET,
          operator_item{ operator_type::TABLE_GET,         "table_get",    primative_operator::table_get,
                        "get a value from table by key_name" }
        },
        { operator_type::TABLE_PUT,
          operator_item{ operator_type::TABLE_PUT,         "table_put",    primative_operator::table_put,
                        "put a value to table" }
        },
    };
}

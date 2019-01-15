#include "operator_type.h"
#include "primative_operator.h"

namespace interpretor {
    operator_item operator_table[] = {
        { operator_type::EMPTY,             "empty",        primative_operator::empty_operator },
        { operator_type::ADDITION,          "add",          primative_operator::addtion },
        { operator_type::SUBTRACTION,       "sub",          primative_operator::subtraction },
        { operator_type::MULTIPLICATION,    "mul",          primative_operator::multiplication },
        { operator_type::DIVISION,          "div",          primative_operator::division },
        { operator_type::TABLE,             "table",        primative_operator::new_table, "create table" },
        { operator_type::TABLE_GET,         "table_get",    primative_operator::table_get, "get a value from table by key_name" },
        { operator_type::TABLE_PUT,         "table_put",    primative_operator::table_put, "put a value to table" },
    };
}

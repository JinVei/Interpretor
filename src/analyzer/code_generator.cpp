#include "plog.h"
#include "code_generator.h"
#include "primitive_label_compiler_registry.h"

namespace interpretor {

    //Register R0 will be used to deposits the return value of the expression
    //Arg[0~9] which be pushed in stack frame(ebp), will be used to deposits the value of parameters

    const char*  get_constant_string(std::string str) {
        g_code_symbol_table[str] = str;
        return g_code_symbol_table[str].c_str();
    }

    static void find_env_instruction(std::string& label, int deep, std::vector<instruction>& target_instruction) {
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::env1) },
            { operand_type::register_operand, register_to_value(register_t::env0) }
        } });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::eax) },
            { operand_type::immediate_operand, value(deep) }
        } });
        target_instruction.push_back({
            operator_type::CMP,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::eax) },
            { operand_type::immediate_operand, value(0.0) }
        } });
        target_instruction.push_back({
            operator_type::BRANCH,
            operand_list{ { operand_type::immediate_operand, value(6), register_to_value(register_t::pc) }
        } });
        target_instruction.push_back({
            operator_type::TABLE_GET,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::env1) },
            { operand_type::immediate_operand, value(SYMBOL_PRE_ENV) }
        } });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::env1) },
            { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
        target_instruction.push_back({
            operator_type::SUBTRACTION,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::eax) },
            { operand_type::immediate_operand, value(1) }
        } });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::eax) },
            { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
        target_instruction.push_back({
            operator_type::JUMP,
            operand_list{ { operand_type::immediate_operand, value(-6), register_to_value(register_t::pc) }
        } });
        // env will be deposited in register env1
    }

    void find_identifier_instruction(std::string& label, int deep, std::vector<instruction>& target_instruction){
        find_env_instruction(label, deep, target_instruction);
        target_instruction.push_back({
            operator_type::TABLE_GET,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::env1) },
                          { operand_type::immediate_operand, value(g_code_symbol_table[label].c_str()) }
        } });
        //the return value will be deposited in register ret
    }

    static void push_stack_frame_instruction(std::vector<instruction>& target_instruction) {
        //push arg table
        target_instruction.push_back({
            operator_type::TABLE,
        });
        target_instruction.push_back({
            operator_type::PUSH,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
        target_instruction.push_back({
            operator_type::ADDITION,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::ebp) },
            { operand_type::immediate_operand, value(1) }
        } });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::ebp) },
            { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
    }

    static void pop_stack_frame_instruction(std::vector<instruction>& target_instruction) {
        //pop arg table
        target_instruction.push_back({
            operator_type::POP
        });
        target_instruction.push_back({
            operator_type::SUBTRACTION,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::ebp) },
            { operand_type::immediate_operand, value(1) }
        } });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::ebp) },
            { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
    }
    static void call_operation_instruction(std::vector<instruction>& target_instruction) {
        //jump to operation addr
        target_instruction.push_back({
            operator_type::TABLE_PUT,
            operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
            { operand_type::immediate_operand, value(SYMBOL_CALLER_ENV) },
            { operand_type::register_operand, register_to_value(register_t::env0) }
        } });
        target_instruction.push_back({
            operator_type::TABLE_GET,
            operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
            { operand_type::immediate_operand, value(SYMBOL_TARGET_ENV) }
        } });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::env0) },
            { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
        target_instruction.push_back({
            operator_type::TABLE_PUT,
            operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
            { operand_type::immediate_operand, value(SYMBOL_RETURN_ADDRESS) },
            { operand_type::immediate_operand, value(3), register_to_value(register_t::pc) }
        } });
        target_instruction.push_back({
            operator_type::TABLE_GET,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::env0) },
            { operand_type::immediate_operand, value(SYMBOL_START_ADDRESS) }
        } });
        target_instruction.push_back({
            operator_type::JUMP,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
        //return_address ,restore env
        target_instruction.push_back({
            operator_type::TABLE_GET,
            operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
            { operand_type::immediate_operand, value(SYMBOL_CALLER_ENV) }
        } });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::env0) },
            { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
    }

    bool code_generator::compile(
        lexical_analyzer::word_expression& expression_tree,
        std::vector<instruction>& target_instruction)
    {
        target_instruction.push_back({
            operator_type::TABLE,
            operand_list{} 
        });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::env0) },
                          { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
        target_instruction.push_back({
            operator_type::PUSH,
            operand_list{{ operand_type::register_operand, register_to_value(register_t::env0)}}
        });

        bool is_ok = false;
        for (auto& expression : expression_tree) {
            if (expression->_type != lexical_analyzer::word::type::expreesion) {
                m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n expression->_type != lexical_analyzer::word::type::expreesion\n";
                return false;
            }
            is_ok = expression_compile(expression->_expression, target_instruction);
            if (!is_ok)
                return false;
        }
        target_instruction.push_back({
            operator_type::SHUT_DOWN,
            operand_list{} });
        return true;
    }

    bool code_generator::expression_compile(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction) {
        //bool is_primitive_operator = false;
        bool is_nomarl = false;
        std::string label;

        auto it_word = expression_tree.begin();
        if (it_word == expression_tree.end()) {
            target_instruction.push_back({
                operator_type::MOV,
                operand_list{ { operand_type::register_address_operand, register_to_value(register_t::ret) },
                              { operand_type::immediate_operand, value() }
            } });
            return true;
        }

        push_stack_frame_instruction(target_instruction);

        auto operation = *it_word;
        if (operation->_type == lexical_analyzer::word::type::label) {
            label = operation->_label;
            if (primitive_label_compiler.find(label) != primitive_label_compiler.end()) {
                bool is_ok = primitive_label_compiler[label](*this, expression_tree, target_instruction);//param
                if (!is_ok)
                    return false;

                pop_stack_frame_instruction(target_instruction);
                return true;
            }
        }

        lexical_analyzer::word_expression::iterator it_word_operand_begin = std::next(it_word, 1);
        lexical_analyzer::word_expression::iterator it_word_operand_end = expression_tree.end();
        operand_compile(it_word_operand_begin, it_word_operand_end, target_instruction);

        is_nomarl = operation_compile(*operation, target_instruction);
        if (!is_nomarl)
            return false;

        call_operation_instruction(target_instruction);

        pop_stack_frame_instruction(target_instruction);
        return true;
    }

    bool code_generator::operation_compile(lexical_analyzer::word& word, std::vector<instruction>& target_instruction) {
        if (word._type == lexical_analyzer::word::type::expreesion) {
            bool is_normal = expression_compile(word._expression, target_instruction);
            if (!is_normal)
                return false;
            target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand, value(SYMBOL_TARGET_ENV) },
                              { operand_type::register_operand, register_to_value(register_t::R0) }
            } });
            return true;
        }
        else if (word._type == lexical_analyzer::word::type::label) {
            std::string label = word._label;
            int deep = find_identifier(label.c_str(), word._compile_time_env_id);
            if (deep < 0) {
                m_error_message = std::string(PLOG_FUNCTION_LOCATION_INFO "\n find_identifier(label.c_str()): deep < 0. label= ") + label + "\n";
                return false;
            }
            find_identifier_instruction(label, deep, target_instruction);
            target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand, value(SYMBOL_TARGET_ENV) },
                              { operand_type::register_operand, register_to_value(register_t::ret) }
            } });
            return true;
        }
        else {
            m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n word->_type is not the type of expression or label\n";
            return false;
        }
    }

    bool code_generator::operand_compile(lexical_analyzer::word_expression::iterator it_operand,
                                        lexical_analyzer::word_expression::iterator it_end, 
                                        std::vector<instruction>& target_instruction) 
    {
        bool is_normal = false;
        int number = 0;

        for (; it_operand != it_end; ++number, it_operand++) {
            std::string arg_label = SYMBOL_ARG(number);
            auto& word = **it_operand;

            if (word._type == lexical_analyzer::word::type::number || word._type == lexical_analyzer::word::type::quoto) {
                value arg_val;
                if (word._type == lexical_analyzer::word::type::number) {
                    arg_val = value(word._number);
                }
                else {
                    arg_val = value(g_code_symbol_table[word._quote].c_str());
                }

                target_instruction.push_back({
                    operator_type::TABLE_PUT,
                    operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                                  { operand_type::immediate_operand,    value(g_code_symbol_table[arg_label].c_str()) },
                                  { operand_type::immediate_operand,     arg_val }
                } });
                //goto Exit1;
            }
            else {
                if (word._type == lexical_analyzer::word::type::expreesion) {
                    is_normal = expression_compile(word._expression, target_instruction);
                    if (!is_normal)
                        goto Error;

                    target_instruction.push_back({
                        operator_type::TABLE_PUT,
                        operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                                      { operand_type::immediate_operand, value(g_code_symbol_table[arg_label].c_str()) },
                                      { operand_type::register_operand, register_to_value(register_t::R0) }
                    } });
                    //goto Exit1;
                }
                else {
                    std::string label = word._label;
                    int deep = find_identifier(label.c_str(), word._compile_time_env_id);
                    if (deep < 0) {
                        m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n find_identifier(label.c_str()): deep < 0\n";
                        goto Error;
                    }
                    find_identifier_instruction(label, deep, target_instruction);
                    target_instruction.push_back({
                    operator_type::TABLE_PUT,
                    operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                                  { operand_type::immediate_operand, value(g_code_symbol_table[arg_label].c_str()) },
                                  { operand_type::register_operand, register_to_value(register_t::ret) }
                    } });
                    //goto Exit1;
                }
            }
        }//for
    //Exit1:
        target_instruction.push_back({
            operator_type::TABLE_PUT,
            operand_list{ { operand_type::stack_operand,  value(0.0),  register_to_value(register_t::ebp) },
            { operand_type::immediate_operand,  value(SYMBOL_ARG_LEN) },
            { operand_type::immediate_operand,  value(number) }
        } });
        return true;
    Error:
        return false;
    }

    int code_generator::find_identifier(const char * label, unsigned int current_env_id)
    {
        int deep = 0;
        bool is_exist;
        unsigned int env_id = current_env_id;

        do {
            is_exist = m_compile_time_env_table[env_id].check_name(label);
            if (is_exist)
                return deep;
            else {
                value pre_env_id = m_compile_time_env_table[env_id].get_value("pre_env_id");
                if (pre_env_id == value())
                    return -1;

                if (pre_env_id.type() != value_type::NUMBER)
                    std::abort();

                env_id = (unsigned int)pre_env_id.number();
                ++deep;
            }
        } while (1);
    }

    code_generator::code_generator(std::vector<table>& compile_time_env_table) {
        m_compile_time_env_table = compile_time_env_table;
    }
}

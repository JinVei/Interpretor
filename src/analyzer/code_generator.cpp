#include "plog.h"
#include "code_generator.h"

namespace interpretor {
    const char*  get_constant_string(std::string str) {
        g_code_symbol_table[str] = str;
        return g_code_symbol_table[str].c_str();
    }
#define SYMBOL_RETURN_ADDRESS   get_constant_string("return_address")
#define SYMBOL_START_ADDRESS    get_constant_string("start_address")
#define SYMBOL_ARG(number)      get_constant_string("arg" + std::to_string(number))
#define SYMBOL_ARG_LEN          get_constant_string("arg_len")
#define SYMBOL_PRE_ENV          get_constant_string("pre_env")
#define SYMBOL_CALLER_ENV       get_constant_string("caller_env")
#define SYMBOL_TARGET_ENV       get_constant_string("target_env")

    bool primative_label_lambda(
        code_generator& code_generator,
        lexical_analyzer::word_expression& lambda_expression,
        std::vector<instruction>& target_instruction
    );

    bool primative_label_define(
        code_generator& code_generator,
        lexical_analyzer::word_expression& lambda_expression,
        std::vector<instruction>& target_instruction
        );

    //primative_label_compiler
    std::unordered_map<std::string, code_generator_handle> primative_label_compiler = {
        { "lambda%", primative_label_lambda },
        { "define%", primative_label_define },
        //set!
    };

    bool primative_label_define(
        code_generator& code_generator,
        lexical_analyzer::word_expression& define_expression,
        std::vector<instruction>& target_instruction )
    {
        if (define_expression.size() < 3) {
            code_generator.m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n define args is insufficient.\n";
            return false;
        }
        auto it_word_name = std::next(define_expression.begin(), 1);
        if ((*it_word_name)->_type != lexical_analyzer::word::type::label) {
            code_generator.m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n the first arg must be label type.\n";
            return false;
        }
        auto it_word_val = std::next(define_expression.begin(), 2);
        if ((*it_word_val)->_type == lexical_analyzer::word::type::expreesion) {
            code_generator.expression_compile((*it_word_val)->_expression, target_instruction);
            target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::register_operand, register_to_value(register_t::env0) },
                              { operand_type::immediate_operand, value(g_code_symbol_table[(*it_word_name)->_label].c_str()) },
                              { operand_type::register_operand, register_to_value(register_t::R0) }
            } });
        }
        else if ((*it_word_val)->_type == lexical_analyzer::word::type::label) {
            target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::register_operand, register_to_value(register_t::env0) },
                              { operand_type::immediate_operand, value(g_code_symbol_table[(*it_word_name)->_label].c_str()) },
                              { operand_type::immediate_operand, value(g_code_symbol_table[(*it_word_val)->_label].c_str()) }
            } });
        }
        else if ((*it_word_val)->_type == lexical_analyzer::word::type::number) {
           target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::register_operand, register_to_value(register_t::env0) },
                              { operand_type::immediate_operand, value(g_code_symbol_table[(*it_word_name)->_label].c_str()) },
                              { operand_type::immediate_operand, value((*it_word_val)->_number) }
            } });
        }
        else if ((*it_word_val)->_type == lexical_analyzer::word::type::quoto) {
            target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::register_operand, register_to_value(register_t::env0) },
                              { operand_type::immediate_operand, value(g_code_symbol_table[(*it_word_name)->_label].c_str()) },
                              { operand_type::immediate_operand, value(g_code_symbol_table[(*it_word_val)->_quote].c_str()) }
            } });
        }
        else {
            code_generator.m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n ((*it_word_val)->_type : unknown type.\n";
            return false;
        }

        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::R0) },
            { operand_type::immediate_operand, value() }
        } });
        return true;
    }
    static bool lambda_body_compile(
        code_generator& code_generator,
        lexical_analyzer::word_expression& lambda_expression,
        std::vector<instruction>& lambda_proc_instruction)
    {
        //lambda arg
        auto it_word_args = std::next(lambda_expression.begin(), 1);
        if (it_word_args == lambda_expression.end()) {
            code_generator.m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n Lambda args is empty.\n";
            return false;
        }
        if ((*it_word_args)->_type != lexical_analyzer::word::type::expreesion) {
            code_generator.m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n Lambda arg list must be expression type.\n";
            return false;
        }

        lambda_proc_instruction.push_back({
            operator_type::TABLE_GET,
            operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                          { operand_type::immediate_operand, value(SYMBOL_ARG_LEN) }
        } });
        lambda_proc_instruction.push_back({
            operator_type::CMP,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::ret) },
                          { operand_type::immediate_operand, value((*it_word_args)->_expression.size()) }
        } });
        lambda_proc_instruction.push_back({
            operator_type::BRANCH,
            operand_list{ { operand_type::immediate_operand, value(3), register_to_value(register_t::pc) },
        } });
        lambda_proc_instruction.push_back({
            operator_type::LOG,
            operand_list{ { operand_type::immediate_operand, value(PLOG_FUNCTION_LOCATION_INFO "\n the length of args mismatch\n") },
        } });
        lambda_proc_instruction.push_back({
            operator_type::SHUT_DOWN,
            operand_list{  } });

        int number = 0;
        for (auto& arg : (*it_word_args)->_expression) {
            if (arg->_type != lexical_analyzer::word::type::label) {
                code_generator.m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n Lambda arg must be label type.\n";
                return false;
            }
            lambda_proc_instruction.push_back({
                operator_type::TABLE_GET,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                { operand_type::immediate_operand, value(SYMBOL_ARG(number)) }
            } });
            lambda_proc_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::register_operand, register_to_value(register_t::env0) },
                { operand_type::immediate_operand, value(g_code_symbol_table[arg->_label].c_str()) },
                { operand_type::register_operand, register_to_value(register_t::ret) }
            } });
            number++;
        }//for
         //lambda body
        bool is_ok = false;
        auto it_word_lambda_body = std::next(lambda_expression.begin(), 2);
        for (; it_word_lambda_body != lambda_expression.end(); it_word_lambda_body++) {
            if ((*it_word_lambda_body)->_type != lexical_analyzer::word::type::expreesion) {
                code_generator.m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n Lambda body must be expression type.\n";
                return false;
            }
            is_ok = code_generator.expression_compile((*it_word_lambda_body)->_expression, lambda_proc_instruction);
            if (!is_ok)
                return false;
        }
        //lambda body return
        lambda_proc_instruction.push_back({
            operator_type::TABLE_GET,
            operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                          { operand_type::immediate_operand, value(SYMBOL_RETURN_ADDRESS) }
        } });
        lambda_proc_instruction.push_back({
            operator_type::JUMP,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::ret) },
        } });
        return true;
    }

    bool primative_label_lambda(
        code_generator& code_generator,
        lexical_analyzer::word_expression& lambda_expression,
        std::vector<instruction>& target_instruction)
    {
        //create lambda env
        target_instruction.push_back({
            operator_type::TABLE,
        });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::R0) },
                          { operand_type::register_operand, register_to_value(register_t::ret) }
        } });
        target_instruction.push_back({
            operator_type::TABLE_PUT,
            operand_list{   { operand_type::register_operand, register_to_value(register_t::R0) },
                            { operand_type::immediate_operand, value(SYMBOL_PRE_ENV) },
                            { operand_type::register_operand, register_to_value(register_t::env0) }
        } });
        target_instruction.push_back({
            operator_type::TABLE_PUT,
            operand_list{   { operand_type::register_operand, register_to_value(register_t::R0) },
                            { operand_type::immediate_operand, value(SYMBOL_START_ADDRESS) },
                            { operand_type::immediate_operand, value(2), register_to_value(register_t::pc) }
        } });
        std::vector<instruction> lambda_proc_instruction;
        lambda_body_compile(code_generator, lambda_expression, lambda_proc_instruction);
        //lambda expreesion return
        target_instruction.push_back({
            operator_type::JUMP,
            operand_list{ { operand_type::immediate_operand, value(lambda_proc_instruction.size() + 1), register_to_value(register_t::pc) },
        } });
        //lambda proc entry
        target_instruction.insert(  target_instruction.end(),
                                    lambda_proc_instruction.begin(),
                                    lambda_proc_instruction.end());
        return true;
    }
    //optimize
    static void find_identifier_instruction(std::string& label, int deep, std::vector<instruction>& target_instruction){
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
        
        target_instruction.push_back({
            operator_type::TABLE_GET,
            operand_list{ { operand_type::register_operand, register_to_value(register_t::env1) },
                          { operand_type::immediate_operand, value(g_code_symbol_table[label].c_str()) }
        } });
    }

    //static void push_table_instruction(std::vector<instruction>& target_instruction) {
    //}

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
        //bool is_primative_operator = false;
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
            if (primative_label_compiler.find(label) != primative_label_compiler.end()) {
                bool is_ok = primative_label_compiler[label](*this, expression_tree, target_instruction);//param
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
                m_error_message = std::string(PLOG_FUNCTION_LOCATION_INFO "\n find_identifier(label.c_str()): deep < 0). label= ") + label + "\n";
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

    code_generator::code_generator(std::vector<table>& compile_time_env_table) {
        m_compile_time_env_table = compile_time_env_table;
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
}

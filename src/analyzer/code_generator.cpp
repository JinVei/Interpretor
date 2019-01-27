#include "plog.h"
#include "code_generator.h"

namespace interpretor {
    void empty(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction) {}

    std::unordered_map<std::string, code_generator_handle> primative_label_compiler = {
        { "+%", empty },
    };

    //optimize
    static void push_find_identifier_instruction(std::string& label, int deep, std::vector<instruction>& target_instruction){
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
                          { operand_type::immediate_operand, value("pre_env") }
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
                          { operand_type::immediate_operand, value(g_identifier[label].c_str()) }
        } });
    }

    void code_generator::compile(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& generated_instruction) {

    }
    bool code_generator::expression_compile(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction) {
        bool is_primative_operator = false;
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
        auto operation = *it_word;

        if (operation->_type == lexical_analyzer::word::type::label) {
            label = operation->_label;
            if (primative_label_compiler.find(label) != primative_label_compiler.end())
                is_primative_operator = true;
        }

        //push arg list table
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
                          { operand_type::immediate_operand, value(1)}
        } });
        target_instruction.push_back({
            operator_type::MOV,
            operand_list{ { operand_type::register_address_operand, register_to_value(register_t::ebp) },
                          { operand_type::register_operand, register_to_value(register_t::ret) }
        } });

        it_word++;
        unsigned int arg_len = 0;
        while (it_word != expression_tree.end()) {
            operand_compile(arg_len, *(*it_word), target_instruction);
        }
        target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::stack_operand,  value(0.0),  register_to_value(register_t::ebp) },
                                { operand_type::immediate_operand,  value(g_identifier["arg_len"].c_str()) },
                                { operand_type::immediate_operand,  value(arg_len) }
        }});

        if (is_primative_operator){
            primative_label_compiler[label](expression_tree, target_instruction);//param
        }
        else {
            is_nomarl = operation_compile(*operation, target_instruction);
            if (!is_nomarl)
                return false;
            //jump to operation addr
            target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand, value("caller_env") },
                              { operand_type::register_operand, register_to_value(register_t::env0) }
            } });
            target_instruction.push_back({
                operator_type::TABLE_GET,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand, value("target_env") }
            } });
            target_instruction.push_back({
                operator_type::MOV,
                operand_list{ { operand_type::register_address_operand, register_to_value(register_t::env0) },
                              { operand_type::register_operand, register_to_value(register_t::ret) }
            } });
            target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand, value("return_address") },
                              { operand_type::immediate_operand, value(3), register_to_value(register_t::pc) }
            } });
            target_instruction.push_back({
                operator_type::TABLE_GET,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand, value("start_address") }
            } });
            target_instruction.push_back({
                operator_type::JUMP,
                operand_list{ {operand_type::register_operand, register_to_value(register_t::ret) }
            } });
            //return_address ,restore env
            target_instruction.push_back({
                operator_type::TABLE_GET,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand, value("caller_env") }
            } });
            target_instruction.push_back({
                operator_type::MOV,
                operand_list{ { operand_type::register_address_operand, register_to_value(register_t::env0) },
                              { operand_type::register_operand, register_to_value(register_t::ret) }
            } });
        }
        
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
                              { operand_type::immediate_operand, value("target_env") },
                              { operand_type::register_operand, register_to_value(register_t::R0) }
            } });
            return true;
        }
        else if (word._type == lexical_analyzer::word::type::label) {
            std::string label = word._label;
            int deep = find_identifier(label.c_str());
            if (deep < 0) {
                m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n find_identifier(label.c_str()): deep < 0\n";
                return false;
            }
            push_find_identifier_instruction(label, deep, target_instruction);
            target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand, value("target_env") },
                              { operand_type::register_operand, register_to_value(register_t::ret) }
            } });
            return true;
        }
        else {
            m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n word->_type is not the type of expression or label\n";
            return false;
        }
    }

    bool code_generator::operand_compile(unsigned int number, 
                                        lexical_analyzer::word & word, 
                                        std::vector<instruction>& target_instruction) 
    {
        bool is_normal = false;
        std::string arg_label = std::string("arg") + std::to_string(number);

        if (word._type == lexical_analyzer::word::type::number || word._type == lexical_analyzer::word::type::quoto) {
            value arg_val;
            if (word._type == lexical_analyzer::word::type::number) {
                arg_val = value(word._number);
            }
            else {
                arg_val = value(g_identifier[arg_label].c_str());
            }

            target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand,    value(g_identifier[arg_label].c_str()) },
                              { operand_type::immediate_operand,     arg_val }
            } });
            return true;
        }
        else {
            if (word._type == lexical_analyzer::word::type::expreesion) {
                is_normal = expression_compile(word._expression, target_instruction);
                if (!is_normal)
                    return false;

                target_instruction.push_back({
                    operator_type::TABLE_PUT,
                    operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                                  { operand_type::immediate_operand, value(g_identifier[arg_label].c_str()) },
                                  { operand_type::register_operand, register_to_value(register_t::R0) }
                }});
                return true;
            }
            else /*if (word._type == lexical_analyzer::word::type::label) */{
                std::string label = word._label;
                int deep = find_identifier(label.c_str());
                if (deep < 0) {
                    m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n find_identifier(label.c_str()): deep < 0\n";
                    return false;
                }
                push_find_identifier_instruction(label, deep, target_instruction);
                target_instruction.push_back({
                operator_type::TABLE_PUT,
                operand_list{ { operand_type::stack_operand, value(0.0), register_to_value(register_t::ebp) },
                              { operand_type::immediate_operand, value(g_identifier[arg_label].c_str()) },
                              { operand_type::register_operand, register_to_value(register_t::ret) }
                } });
                return true;
            }
        }
    }
}

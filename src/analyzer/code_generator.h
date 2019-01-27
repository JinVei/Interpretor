#ifndef __INTERPRETOR_CODE_GENERATOR_H
#define __INTERPRETOR_CODE_GENERATOR_H
#include "instruction.h"
#include "lexical_analyzer.h"

#include <unordered_map>
#include <functional>
namespace interpretor {
    using code_generator_handle = std::function<void(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction)>;
    extern std::unordered_map<std::string, code_generator_handle> label_recognazation_table;

    class code_generator {
        std::string m_error_message;

        void compile(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction);
        bool expression_compile(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction);
        bool operation_compile(lexical_analyzer::word& word, std::vector<instruction>& target_instruction);
        bool operand_compile(unsigned int number, lexical_analyzer::word& word, std::vector<instruction>& target_instruction);
        void primative_operator_compile();
        void lambda_compile();
        void define_compile();
        int find_identifier(const char* label);
    };
}
#endif
#ifndef __INTERPRETOR_CODE_GENERATOR_H
#define __INTERPRETOR_CODE_GENERATOR_H
#include "instruction.h"
#include "lexical_analyzer.h"

#include <unordered_map>
#include <functional>
namespace interpretor {
    //extern std::unordered_map<std::string, code_generator_handle> label_recognazation_table;

    class code_generator {
    public:
        std::string m_error_message;
        lexical_analyzer&    m_analyze;
        code_generator(lexical_analyzer&    analyze) : m_analyze(analyze) {};

        void compile(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction);
        bool expression_compile(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction);
        bool operation_compile(lexical_analyzer::word& word, std::vector<instruction>& target_instruction);

        bool code_generator::operand_compile(
            lexical_analyzer::word_expression::iterator it_operand,
            lexical_analyzer::word_expression::iterator it_end,
            std::vector<instruction>& target_instruction
        );
        int find_identifier(const char* label, unsigned int current_env_id);
        void primative_operator_compile();
        void lambda_compile();
        void define_compile();
    };
    using code_generator_handle = std::function<bool(code_generator& generator,
        lexical_analyzer::word_expression& expression,
        std::vector<instruction>& target_instruction)>;
}
#endif
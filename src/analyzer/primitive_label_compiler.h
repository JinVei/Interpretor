#ifndef __INTERPRETOR_PRIMITIVE_LABEL_COMPILER_H
#define __INTERPRETOR_PRIMITIVE_LABEL_COMPILER_H
#include "code_generator.h"

namespace interpretor {
    bool define_compile(
        code_generator& code_generator,
        lexical_analyzer::word_expression& lambda_expression,
        std::vector<instruction>& target_instruction);

    bool lambda_compile(
        code_generator& code_generator,
        lexical_analyzer::word_expression& lambda_expression,
        std::vector<instruction>& target_instruction);

    bool log_compile(
        code_generator& code_generator,
        lexical_analyzer::word_expression& expression,
        std::vector<instruction>& target_instruction);

    bool addtion_compile(
        code_generator& code_generator,
        lexical_analyzer::word_expression& expression,
        std::vector<instruction>& target_instruction);

    bool subtraction_compile(
        code_generator& code_generator,
        lexical_analyzer::word_expression& expression,
        std::vector<instruction>& target_instruction);

    bool multiplication_compile(
        code_generator& code_generator,
        lexical_analyzer::word_expression& expression,
        std::vector<instruction>& target_instruction);

    bool division_compile(
        code_generator& code_generator,
        lexical_analyzer::word_expression& expression,
        std::vector<instruction>& target_instruction);

    bool assign_compile(
        code_generator& code_generator,
        lexical_analyzer::word_expression& expression,
        std::vector<instruction>& target_instruction);
}

#endif

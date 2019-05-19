#ifndef __INTERPRETOR_CODE_GENERATOR_H
#define __INTERPRETOR_CODE_GENERATOR_H
#include "instruction.h"
#include "lexical_analyzer.h"

#include <functional>

namespace interpretor {
    //extern std::unordered_map<std::string, code_generator_handle> label_recognazation_table;

    class code_generator {
    public:
        std::string m_error_message;
        std::vector<table>  m_compile_time_env_table;
        code_generator(std::vector<table>&  compile_time_env_table);

        bool compile(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction);
        bool expression_compile(lexical_analyzer::word_expression& expression_tree, std::vector<instruction>& target_instruction);
        bool operation_compile(lexical_analyzer::word& word, std::vector<instruction>& target_instruction);

        bool code_generator::operand_compile(
            lexical_analyzer::word_expression::iterator it_operand,
            lexical_analyzer::word_expression::iterator it_end,
            std::vector<instruction>& target_instruction
        );
        int find_identifier(const char* label, unsigned int current_env_id);
    };

    using code_generator_handle = std::function<bool(code_generator& generator,
                                                    lexical_analyzer::word_expression& expression,
                                                    std::vector<instruction>& target_instruction)>;

    const char*  get_constant_string(std::string str);

#define SYMBOL_RETURN_ADDRESS   get_constant_string("return_address")
#define SYMBOL_START_ADDRESS    get_constant_string("start_address")
#define SYMBOL_ARG(number)      get_constant_string("arg" + std::to_string(number))
#define SYMBOL_ARG_LEN          get_constant_string("arg_len")
#define SYMBOL_PRE_ENV          get_constant_string("pre_env")
#define SYMBOL_CALLER_ENV       get_constant_string("caller_env")
#define SYMBOL_TARGET_ENV       get_constant_string("target_env")

    void find_identifier_instruction(std::string& label, int deep, std::vector<instruction>& target_instruction);

}
#endif
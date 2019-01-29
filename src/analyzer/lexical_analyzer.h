#ifndef __LEXICAL_ANALYZER_H
#define __LEXICAL_ANALYZER_H
#include "table_type.h"
#include <list>
#include <functional>
#include <string>
#include <memory>
#include <map>

namespace interpretor {
    class lexical_analyzer {
    public:
        struct word;
        using word_expression = std::list<std::shared_ptr<word>>;

        struct word {
            enum class type {
                expreesion = 0,
                number,
                label,
                quoto
            };

            word_expression     _expression;
            double              _number;
            std::string         _label;
            std::string         _quote;

            type                _type;
            word*               _pre_word = nullptr;

            unsigned int                        _compile_time_env_id;
            std::list<std::function<void()>>    _parser_done_hander_list;

            word() {};
            word(word_expression expression);
            word(double number);
            word(type type, std::string str);

        };

    private:
        bool                        m_error_flag    = false;
        int                         m_status        = 0;
        const char*                 m_text          = nullptr;
        unsigned int                m_text_length   = 0;
        unsigned int                m_text_index    = 0;
        std::function<void()>       m_error_handler;
        word                        m_lexical_tree;

        void finite_state();
        auto expression_state() -> std::shared_ptr<word>;
        auto number_state()     -> std::shared_ptr<word>;
        auto quote_state()      -> std::shared_ptr<word>;
        auto label_state()      -> std::shared_ptr<word>;
        auto label_state(std::string& label)->std::shared_ptr<word>;

        bool parser(word expression);
    public:
        const char*         m_error_message = nullptr;
        std::vector<table>  m_compile_time_env_table;
        unsigned int        m_current_env_id;

        lexical_analyzer(const char* text, unsigned int text_len);
        bool do_analysis();
        word& get_lexical_tree();
    };

    extern std::map<std::string, std::string> g_code_symbol_table;
}
#endif
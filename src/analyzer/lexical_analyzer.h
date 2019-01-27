#ifndef __LEXICAL_ANALYZER_H
#define __LEXICAL_ANALYZER_H
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
        //struct phrase {
        //    std::list<std::shared_ptr<word>> _words;
        //};

        struct word {
            enum class type {
                expreesion = 0,
                number,
                label,
                quoto
            };
//            union {
            word_expression     _expression;// = nullptr;
            double              _number;
            std::string         _label;
            std::string         _quote;
//            };

            type _type;

            //word& operator=(word&) = delete;

            word(word_expression expression);
            word(double number);
            word(type type, std::string str);

//            virtual ~word();
        };

    private:
        bool                        m_error_flag    = false;
        int                         m_status        = 0;
        const char*                 m_text          = nullptr;
        unsigned int                m_text_length   = 0;
        unsigned int                m_text_index    = 0;
        std::function<void()>       m_error_handler;
        word_expression             m_expression_tree;

        void finite_state();
        auto expression_state() -> std::shared_ptr<word>;
        auto number_state()     -> std::shared_ptr<word>;
        auto quote_state()      -> std::shared_ptr<word>;
        auto label_state()      -> std::shared_ptr<word>;
        auto label_state(std::string& label)->std::shared_ptr<word>;

        void parser(word_expression expression);
    public:
        const char*     m_error_message = nullptr;

        lexical_analyzer(const char* text, unsigned int text_len);
        bool do_analysis();
    };

    extern std::map<std::string, std::string> g_identifier;
}
#endif
#ifndef __LEXICAL_ANALYZER_H
#define __LEXICAL_ANALYZER_H
#include <list>
#include <functional>
#include <string>
#include <memory>

namespace interpretor {
    class lexical_analyzer {
        struct word;

        struct phrase {
            std::list<std::shared_ptr<word>> _words;
        };

        struct word {
            enum class type {
                expreesion = 0,
                number,
                label,
                quoto
            };
            union {
                phrase*                 _expression = nullptr;
                double                  _number;
                std::string*            _label;
                std::string*            _quote;
            };
            type _type;

            word(phrase* expression);
            word(double number);
            word(type type, std::string* str);

            virtual ~word();
        };

        bool                        m_error_flag    = false;
        int                         m_status        = 0;
        const char*                 m_text          = nullptr;
        unsigned int                m_text_length   = 0;
        unsigned int                m_text_index    = 0;
        std::function<void()>       m_error_handler;
        phrase                      m_expression_tree;

        void finite_state();
        auto expression_state() -> std::shared_ptr<word>;
        auto number_state()     -> std::shared_ptr<word>;
        auto quote_state()      -> std::shared_ptr<word>;
        auto label_state()      -> std::shared_ptr<word>;
        auto label_state(std::string& label)->std::shared_ptr<word>;

    public:
        const char*     m_error_message = nullptr;

        lexical_analyzer(const char* text, unsigned int text_len);
        bool do_analysis();
    };
}
#endif
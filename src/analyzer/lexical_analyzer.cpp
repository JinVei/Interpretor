#include "lexical_analyzer.h"
#include "plog.h"

namespace interpretor {
    std::map<std::string, std::string> g_identifier;

    lexical_analyzer::word::word(word_expression expression) {
        _type = type::expreesion;
        _expression = expression;
    }
    lexical_analyzer::word::word(double number) {
        _type = type::number;
        _number = number;
    }

    lexical_analyzer::word::word(type type, std::string str) {
        if (type == word::type::label)
            _label = str;
        else if (type == word::type::quoto)
            _quote = str;
        else
            std::abort();

        _type = type;
    }

    void lexical_analyzer::finite_state() {
        std::shared_ptr<word> word_ptr;
        m_expression_tree._type = word::type::expreesion;

        while (!m_error_flag) {
            if (m_text_length <= m_text_index) {
                return;
            }

            switch (m_text[m_text_index]) {
            case '\n':
            case '\r':
            case ' ':
                ++m_text_index;
                break;

            case '(':
                ++m_text_index;
                word_ptr = expression_state();
                m_expression_tree._expression.push_back(word_ptr);
                break;

            default:
                m_error_flag = true;
                if (m_error_handler != nullptr)
                    m_error_handler();
                m_error_message = PLOG_FUNCTION_LOCATION_INFO "\nget into default case";
                return;
            }
        }
    }

    auto lexical_analyzer::expression_state() -> std::shared_ptr<word> {
        word_expression expression;// = new phrase();
        std::shared_ptr<word> tmp_val;

        while (!m_error_flag) {
            if (m_text_length <= m_text_index) {
                m_error_message = PLOG_FUNCTION_LOCATION_INFO "\nif (m_text_length <= m_text_index)";
                goto ErrorExit;
            }
            switch (m_text[m_text_index]) {
            case '\n':
            case '\r':
            case ' ':
                ++m_text_index;
                break;

            case '(':
                ++m_text_index;
                tmp_val = expression_state();
                expression.push_back(tmp_val);
                break;

            case ')':
                ++m_text_index;
                return std::shared_ptr<word>(new word(expression));

            case '\'':
                ++m_text_index;
                tmp_val = quote_state();
                expression.push_back(tmp_val);
                break;

            default:
                if ('0' <= m_text[m_text_index] && m_text[m_text_index] <= '9') {
                    tmp_val = number_state();
                    expression.push_back(tmp_val);
                    break;

                } else if (('a' <= m_text[m_text_index] && m_text[m_text_index] <= 'z')
                      || ('A' <= m_text[m_text_index] && m_text[m_text_index] <= 'Z'))
                {
                    tmp_val = label_state();
                    expression.push_back(tmp_val);
                    break;
                } else {
                    m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n no matching case ";
                    goto ErrorExit;
                }
            }
        }
    ErrorExit:
        m_error_flag = true;
        if (m_error_handler != nullptr)
            m_error_handler();
        return std::shared_ptr<word>();
    }

    auto lexical_analyzer::number_state() -> std::shared_ptr<word> {
        std::string str_number;
        bool had_decimal = false;
        double number = 0;

        while (!m_error_flag) {
            if (m_text_length <= m_text_index) {
                m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n if (m_text_length <= m_text_index)";
                goto ErrorExit;
            }
            switch (m_text[m_text_index]) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                str_number.push_back(m_text[m_text_index]);
                ++m_text_index;
                break;

            case '.':
                if (had_decimal)
                    return label_state(str_number);

                had_decimal = true;
                str_number.push_back(m_text[m_text_index]);
                ++m_text_index;
                break;

            case ' ':
            case '\r':
            case '\n':
            case ')':
                number = std::stod(str_number);
                return std::shared_ptr<word>(new word(number));

            default:
                return label_state(str_number);
            }
        }
    ErrorExit:
        m_error_flag = true;
        return std::shared_ptr<word>();
    }

    auto lexical_analyzer::quote_state() -> std::shared_ptr<word> {
        std::string quoto;

        while (!m_error_flag) {
            if (m_text_length <= m_text_index) {
                m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n if (m_text_length <= m_text_index)";
                goto ErrorExit;
            }
            switch (m_text[m_text_index]) {
            case '\r':
            case '\n':
            case ' ' :
            case ')':
                return std::shared_ptr<word>(new word(word::type::quoto,std::string(std::move(quoto))));

            default:
                //[!] to [~]
                if ( 0x21 <= m_text[m_text_index] && m_text[m_text_index] <= 0x7E) {
                    quoto.push_back(m_text[m_text_index]);
                    ++m_text_index;
                    break;

                } else{
                    m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n no matching case";
                    goto ErrorExit;
                }
            }
        }

    ErrorExit:
        if (m_error_handler != nullptr)
            m_error_handler();
        m_error_flag = true;
        return std::shared_ptr<word>();
    }

    auto lexical_analyzer::label_state() -> std::shared_ptr<word> {
        std::string label;

        while (!m_error_flag) {
            if (m_text_length <= m_text_index) {
                m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n if (m_text_length <= m_text_index)";
                goto ErrorExit;
            }
            switch (m_text[m_text_index]) {
            case '\r':
            case '\n':
            case ' ':
            case ')':
                return std::shared_ptr<word>(new word(word::type::label, std::string(std::move(label))));

            default:
                //[!] to [~]
                if (0x21 <= m_text[m_text_index] && m_text[m_text_index] <= 0x7E) {
                    label.push_back(m_text[m_text_index]);
                    ++m_text_index;
                    break;

                } else {
                    m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n no matching case";
                    goto ErrorExit;
                }
            }
        }
    ErrorExit:
        if (m_error_handler != nullptr)
            m_error_handler();
        m_error_flag = true;
        return std::shared_ptr<word>();
    }
    auto lexical_analyzer::label_state(std::string & label) -> std::shared_ptr<word> {

        while (!m_error_flag) {
            if (m_text_length <= m_text_index) {
                m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n if (m_text_length <= m_text_index)";
                goto ErrorExit;
            }
            switch (m_text[m_text_index]) {
            case '\r':
            case '\n':
            case ' ':
            case ')':
                return std::shared_ptr<word>(new word(word::type::label, std::string(std::move(label))));

            default:
                //[!] to [~]
                if (0x21 <= m_text[m_text_index] && m_text[m_text_index] <= 0x7E) {
                    label.push_back(m_text[m_text_index]);
                    ++m_text_index;
                    break;

                } else {
                    m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n no matching case";
                    goto ErrorExit;
                }
            }
        }
    ErrorExit:
        if (m_error_handler != nullptr)
            m_error_handler();
        m_error_flag = true;
        return std::shared_ptr<word>();
    }

    static void dispose_label_suffix(lexical_analyzer::word& word) {
        word._label.push_back('%');
        g_identifier[word._label] = word._label;
    }

    static void dispose_lambda_label(lexical_analyzer& analyzer, lexical_analyzer::word& word) {
        if (word._label == "lambda%") {
            unsigned int pre_env_id = analyzer.m_current_env_id;
            
            analyzer.m_compile_time_env_table.push_back(table());
            analyzer.m_current_env_id = analyzer.m_compile_time_env_table.size() - 1;

            table& current_env = analyzer.m_compile_time_env_table[analyzer.m_current_env_id];
            current_env.put_value("pre_env_id", value(pre_env_id));

            lexical_analyzer::word* pre_word = word._pre_word;
            pre_word->_parser_done_hander_list.push_back(
                [&analyzer, pre_env_id]() {
                    analyzer.m_current_env_id = pre_env_id;
                } 
            );
        }
    }

    static bool dispose_define_label(lexical_analyzer& analyzer, lexical_analyzer::word_expression::iterator& it_word) {
        if ((*it_word)->_label == "define%") {
            auto it_next_word = std::next(it_word, 1);
            if ((*it_next_word)->_type != lexical_analyzer::word::type::label) {
                analyzer.m_error_message = PLOG_FUNCTION_LOCATION_INFO "\n The defining identifier is not a label type\n";
                return false;
            }
            std::string identifier = (*it_next_word)->_label;
            g_identifier[identifier] = identifier;
            table& env = analyzer.m_compile_time_env_table[analyzer.m_current_env_id];
            env.put_value(identifier.c_str(), value(g_identifier[identifier].c_str()));
        }
        return true;
    }

    bool lexical_analyzer::parser(word expression) {
        bool is_ok;
        auto it_word = expression._expression.begin();
        for (; it_word != expression._expression.end(); ) {
            (*(*it_word))._pre_word = &expression;
            (*(*it_word))._compile_time_env_id = m_current_env_id;

            if ((*it_word)->_type == word::type::expreesion) {
                is_ok = parser((*it_word)->_expression);
                if (!is_ok) return false;
            }
            else if ((*it_word)->_type == word::type::label) {
                dispose_label_suffix(*(*it_word));
                if ((*it_word) == expression._expression.front()) {
                    dispose_lambda_label(*this, *(*it_word));
                    is_ok = dispose_define_label(*this, it_word);
                    if (!is_ok) return false;
                }
            }
        }

        for (auto handler : expression._parser_done_hander_list) {
            handler();
        }

        return true;
    }

    lexical_analyzer::lexical_analyzer(const char * text, unsigned int text_len) {
        m_text = text;
        m_text_length = text_len;
    }

    bool lexical_analyzer::do_analysis() {
        finite_state();

        m_compile_time_env_table.clear();
        m_compile_time_env_table.push_back(table());
        m_current_env_id = m_compile_time_env_table.size() - 1;
        m_expression_tree._compile_time_env_id = m_current_env_id;
        m_error_flag = parser(m_expression_tree);

        return m_error_flag;
    }
}
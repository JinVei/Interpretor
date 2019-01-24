#include "lexical_analyzer.h"
#include "plog.h"
namespace interpretor {
    lexical_analyzer::word::word(phrase * expression) {
        _type = type::expreesion;
        _expression = expression;
    }
    lexical_analyzer::word::word(double number) {
        _type = type::number;
        _number = number;
    }

    lexical_analyzer::word::word(type type, std::string* str) {
        if (type == word::type::label)
            _label = str;
        else if (type == word::type::quoto)
            _quote = str;
        else
            std::abort();

        _type = type;
    }
    lexical_analyzer::word::~word() {
        if (_type == word::type::expreesion && _expression != nullptr) {
            delete _expression;
        }
        else if (_type == word::type::label && _label != nullptr) {
            delete _label;
        }
        else if (_type == word::type::quoto && _quote != nullptr) {
            delete _quote;
        }
    }

    void lexical_analyzer::finite_state() {
        std::shared_ptr<word> word_ptr;

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
                m_expression_tree._words.push_back(word_ptr);
                break;;

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
        phrase* expression = new phrase();
        std::shared_ptr<word> tmp_val;

        while (!m_error_flag) {
            if (m_text_length <= m_text_index) {
                m_error_message = PLOG_FUNCTION_LOCATION_INFO "\nif (m_text_length <= m_text_index)";
                goto ErrorExit;
            }
            switch (m_text[m_text_index]) {
            case '(':
                ++m_text_index;
                tmp_val = expression_state();
                expression->_words.push_back(tmp_val);
                break;

            case ')':
                ++m_text_index;
                return std::shared_ptr<word>(new word(expression));

            case '\'':
                ++m_text_index;
                tmp_val = quote_state();
                expression->_words.push_back(tmp_val);
                break;

            case '\n':
            case '\r':
            case ' ':
                ++m_text_index;
                break;

            default:
                if ('0' <= m_text[m_text_index] && m_text[m_text_index] <= '9') {
                    tmp_val = number_state();
                    expression->_words.push_back(tmp_val);
                    break;

                } else if (('a' <= m_text[m_text_index] && m_text[m_text_index] <= 'z')
                      || ('A' <= m_text[m_text_index] && m_text[m_text_index] <= 'Z'))
                {
                    tmp_val = label_state();
                    expression->_words.push_back(tmp_val);
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
                return std::shared_ptr<word>(new word(word::type::quoto, new std::string(std::move(quoto))));

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
                return std::shared_ptr<word>(new word(word::type::label, new std::string(std::move(label))));

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
                return std::shared_ptr<word>(new word(word::type::label, new std::string(std::move(label))));

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

    lexical_analyzer::lexical_analyzer(const char * text, unsigned int text_len) {
        m_text = text;
        m_text_length = text_len;
    }

    bool lexical_analyzer::do_analysis() {
        finite_state();
        return m_error_flag;
    }
}
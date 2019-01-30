#include "plog.h"
#include "lexical_analyzer.h"
#include "code_generator.h"
#include "machine.h"

#include <iostream>
using namespace interpretor;
class printer : public interpretor::logger {
    void log(const char* message) override {
        std::cout << message << std::endl;
    }
};

const char* script = "( he waked me up at 7:30 o'clock (88.0.9 99.09 1333))\
                      (mio is so cuted and \
                                            (sound so fine))";
const char* script1 = \
"(define pi 3.14)\
(define func (lambda (x) ()) )\
(func pi)";

const char* script3 = \
"(define pi 3.14)\
(define result 0)\
(= result (+ pi 1))\
(print result)\
(= result (* result 2))\
(print result)\
(= result (- result 5))\
(print result)\
(= result (/ result 2))\
(print result)\
(= result (* result 4))\
(print result)";

void main() {
    std::vector<instruction> target_instruction;
    bool is_ok = false;
    lexical_analyzer analyzer(script3, std::strlen(script3));
    is_ok = analyzer.do_analysis();
    if (!is_ok) {
        std::cout <<analyzer.m_error_message << std::endl;
        getchar();
    }
    code_generator compiler(analyzer.m_compile_time_env_table);
    is_ok = compiler.compile(analyzer.get_lexical_tree()._expression, target_instruction);
    if (!is_ok) {
        std::cout<< compiler.m_error_message << std::endl;
        getchar();
        return;
    }

    printer logger;
    machine evaluator(&logger);
    evaluator.set_repertoire(target_instruction);
    evaluator.run();

    getchar();
}
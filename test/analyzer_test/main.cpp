#include "plog.h"
#include "lexical_analyzer.h"
#include <iostream>
using namespace interpretor;

const char* script = "( he waked me up at 7:30 o'clock (88.0.9 99.09 1333))\
                      (mio is so cuted and \
                                            (sound so fine))";

void main() {
    lexical_analyzer analyzer(script, std::strlen(script));
    analyzer.do_analysis();
    getchar();
}
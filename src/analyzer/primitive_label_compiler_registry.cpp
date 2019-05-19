#include "primitive_label_compiler_registry.h"
#include "primitive_label_compiler.h"

namespace interpretor {
    //Register R0 will be used to deposits the return value of the expression
    //Arg[0~9] which be pushed in stack frame(ebp), will be used to deposits the value of parameters

    //primitive_label_compiler
    std::unordered_map<std::string, code_generator_handle> primitive_label_compiler_registry = {
        { "lambda%", lambda_compile },
        { "define%", define_compile },
        { "+%",      addtion_compile },
        { "-%",      subtraction_compile },
        { "*%",      multiplication_compile },
        { "/%",      division_compile },
        { "=%",      assign_compile },
        { "print%",  log_compile },
        //set!
    };
}
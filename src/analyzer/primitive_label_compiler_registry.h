#ifndef __INTERPRETOR_PRIMITIVE_LABEL_COMPILER_REGISTRY_H
#define __INTERPRETOR_PRIMITIVE_LABEL_COMPILER_REGISTRY_H

#include "code_generator.h"
#include <unordered_map>

namespace interpretor {
    extern std::unordered_map<std::string, code_generator_handle> primitive_label_compiler;
}

#endif

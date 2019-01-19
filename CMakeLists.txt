cmake_minimum_required(VERSION 3.0) 

project(SchemeInterpretor)
set(M_LIB_SRC_DIR ./src)

include_directories(./src/)

add_subdirectory(./src/)
add_subdirectory(./test/primative_instruction_test/)
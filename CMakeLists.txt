cmake_minimum_required(VERSION 3.0) 

project(scheme_interpretor)
set(PROJECT_ROOT_DIR ./)

file(GLOB SRC ${PROJECT_ROOT_DIR}/src/*.cpp)
file(GLOB HDR ${PROJECT_ROOT_DIR}/src/*.h)

source_group("source_file" FILES ${SRC})
source_group("header_file" FILES ${HDR})

include_directories(${PROJECT_ROOT_DIR}/src)

add_executable(scheme_interpretor ${SRC} ${HDR})
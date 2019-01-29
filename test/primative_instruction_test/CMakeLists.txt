cmake_minimum_required(VERSION 3.0) 

file(GLOB SRC  ./*.cpp)
file(GLOB HDR ./*.h)

if( WIN32 )
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(Test_Primative_Instruction ${SRC} ${HDR})
ADD_DEPENDENCIES(Test_Primative_Instruction libSchemeInterpretor)
TARGET_LINK_LIBRARIES(Test_Primative_Instruction libSchemeInterpretor)
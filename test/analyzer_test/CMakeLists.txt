cmake_minimum_required(VERSION 3.0) 

file(GLOB SRC  ./*.cpp)
file(GLOB HDR ./*.h)

if( WIN32 )
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(Analyzer_Test ${SRC} ${HDR})
ADD_DEPENDENCIES(Analyzer_Test libSchemeInterpretor)
TARGET_LINK_LIBRARIES(Analyzer_Test libSchemeInterpretor)
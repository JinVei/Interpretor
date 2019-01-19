cmake_minimum_required(VERSION 3.0) 

file(GLOB SRC  ./*.cpp)
file(GLOB HDR ./*.h)

source_group("source_file" FILES ${SRC})
source_group("header_file" FILES ${HDR})

if( WIN32 )
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#add_executable(SchemeInterpretor ${SRC} ${HDR})
ADD_LIBRARY(libSchemeInterpretor   STATIC   ${SRC} ${HDR})
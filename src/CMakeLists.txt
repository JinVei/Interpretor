cmake_minimum_required(VERSION 3.0) 

file(GLOB M_SRC  ./*.cpp)
file(GLOB M_HDR ./*.h)

source_group("machine_source" FILES ${M_SRC})
source_group("machine_header" FILES ${M_HDR})

file(GLOB A_SRC  ./analyzer/*.cpp)
file(GLOB A_HDR ./analyzer/*.h)

source_group("analyzer_source" FILES ${A_SRC})
source_group("analyzer_header" FILES ${A_HDR})

if( WIN32 )
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

ADD_LIBRARY(libSchemeInterpretor   STATIC   ${M_SRC} ${A_SRC} ${M_HDR} ${A_HDR})
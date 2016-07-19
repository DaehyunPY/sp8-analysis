cmake_minimum_required(VERSION 3.3)
project(JSONHandler)

if((CMAKE_SYSTEM_NAME MATCHES "Linux") OR (CMAKE_SYSTEM_NAME MATCHES "Darwin"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
set(SOURCE_FILES
    JSONFlag.cpp
    JSONReader.cpp
    )
add_library(JSONHandler STATIC ${SOURCE_FILES})

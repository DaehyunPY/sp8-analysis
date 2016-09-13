cmake_minimum_required(VERSION 2.8)
project(Protocols)

if ((CMAKE_SYSTEM_NAME MATCHES "Linux") OR (CMAKE_SYSTEM_NAME MATCHES "Darwin"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64")
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    # add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()
set(SOURCE_FILES
        Flag.cpp
        Hist.cpp
        JSONFlag.cpp
        JSONReader.cpp
        Unit.cpp
        )
add_library(Protocols STATIC ${SOURCE_FILES})

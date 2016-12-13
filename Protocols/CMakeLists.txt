cmake_minimum_required(VERSION 2.8)
project(Protocols)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64 -O3")
set(SOURCE_FILES
    Flag.h Flag.cpp
    Hist.h Hist.cpp
    JSONFlag.h JSONFlag.cpp
    JSONReader.h JSONReader.cpp
    Unit.h Unit.cpp
    PhysicalQuantity.h
)
add_library(Protocols STATIC ${SOURCE_FILES})

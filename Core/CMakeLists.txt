cmake_minimum_required(VERSION 3.6)
project(SP8Core)

set(SOURCE_FILES
        Flag.cpp
        Hist.cpp
        JSONReader.cpp
        Unit.cpp
        )
add_library(sp8core STATIC ${SOURCE_FILES})

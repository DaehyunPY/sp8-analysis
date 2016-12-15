CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Macros)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(SOURCE_FILES
    usr/Draw.cpp
    usr/FitGauss.cpp
    usr/Mark.cpp
    usr/Proj.cpp
)
ADD_LIBRARY(Macros STATIC ${SOURCE_FILES})

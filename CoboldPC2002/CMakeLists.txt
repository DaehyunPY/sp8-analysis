cmake_minimum_required(VERSION 3.3)
project(CoboldPC2002)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    AnalysisDAN.cpp
    CDan.cpp
    CDan.h
    CDAQ.h
    CheckProcess.H
    CoboldDll.h
    DanUtilities.cpp
    DanUtilities.h
    EmptyDAN.cpp
    LMInfo.h
    Resource.h
    SoftwareDependent.cpp
    StandardDAN.cpp
    StdAfx.cpp
    StdAfx.h)

add_executable(CoboldPC2002 ${SOURCE_FILES})
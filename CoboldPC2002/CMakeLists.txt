cmake_minimum_required(VERSION 3.3)
project(CoboldPC2002)

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories("${PROJECT_SOURCE_DIR}/../include")

set(SOURCE_FILES
    Res/CDan.rc2
    AnalysisDAN.cpp
    CDan.cpp
    CDan.h
    CDan.rc
    CDAQ.h
    CheckProcess.H
    CoboldDll.h
    DanUtilities.cpp
    DanUtilities.h
    LMInfo.h
    Resource.h
    SoftwareDependent.cpp
    StdAfx.cpp
    StdAfx.h
    )

add_library(CDan SHARED ${ANALYSIS_CORE} ${SOURCE_FILES})
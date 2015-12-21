cmake_minimum_required(VERSION 3.3)
project(BL17_Analysis)

### core setting
### note: it's only for windows
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MFC_FLAG 2)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
add_definitions(-D_AFXDLL)
add_definitions(-DFOR_COBOLDPC2002)
include_directories("${PROJECT_SOURCE_DIR}")
set(BL17_ANALYSIS_CORE
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
    SoftwareDependent.h
    StdAfx.cpp
    StdAfx.h
    )

### install library
add_library(CDan SHARED
#    WIN32
    ${ANALYSIS_CORE}
    ${BL17_ANALYSIS_CORE}
    )
install(TAGETS CDan LIBRARY DESTINATION "lib")
add_subdirectory(Analysis)
target_link_libraries(CDan AnalysisSharedLib)

cmake_minimum_required(VERSION 2.8)
project(Analysis)

### link ROOT
set(ROOTSYS "$ENV{ROOTSYS}")
include_directories("${ROOTSYS}/include")
link_directories("${ROOTSYS}/lib")
link_libraries(m dl)
link_libraries(
    Core
    RIO
    Net
    Hist
    Graf
    Graf3d
    Gpad
    Tree
    Rint
    Postscript
    Matrix
    Physics
    MathCore
    Thread
)

### link resort
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/resort")
link_libraries(libResort64c_x64.a)

### add sub lib
add_subdirectory(AnalysisCore)
add_subdirectory(Protocols)

### compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64 -O3 -pthread")

### AnalysisExe
set(ANALYSISEXE_SOURCE_FILES
    AnalysisExe/Main.cpp
    AnalysisExe/AnalysisRun.cpp
)
add_executable(AnalysisExe ${ANALYSISEXE_SOURCE_FILES})
target_link_libraries(AnalysisExe AnalysisCore Protocols)

### SortExe
set(SORTEXE_SOURCE_FILES
    SortExe/LMF_IO.cpp
    SortExe/Main.cpp
    SortExe/SortRun.cpp
)
add_executable(SortExe ${SORTEXE_SOURCE_FILES})
target_link_libraries(SortExe Protocols)

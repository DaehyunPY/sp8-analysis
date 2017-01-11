cmake_minimum_required(VERSION 2.8)
project(Analysis)

### link ROOT
execute_process(COMMAND root-config --incdir OUTPUT_VARIABLE INCDIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libdir OUTPUT_VARIABLE LIBDIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --auxcflags OUTPUT_VARIABLE CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(${INCDIR})
link_directories(${LIBDIR})
link_libraries(
    Core RIO Net
    Hist Graf Graf3d Gpad
    Tree Rint Postscript
    Matrix Physics MathCore
    Thread MultiProc
    pthread m dl
)

### link resort
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/resort")
link_libraries(libResort64c_x64.a)

### add sub lib
add_subdirectory(AnalysisCore)
add_subdirectory(Protocols)

### compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS}")

### SortExe
set(SORTEXE_SOURCE_FILES
    SortExe/LMF_IO.cpp
    SortExe/Main.cpp
    SortExe/SortRun.cpp
)
add_executable(SortExe ${SORTEXE_SOURCE_FILES})
target_link_libraries(SortExe Protocols)

### AnalysisExe
set(ANALYSISEXE_SOURCE_FILES
    AnalysisExe/Main.cpp
    AnalysisExe/AnalysisRun.cpp
)
add_executable(AnalysisExe ${ANALYSISEXE_SOURCE_FILES})
target_link_libraries(AnalysisExe AnalysisCore Protocols)

cmake_minimum_required(VERSION 3.6)
project(SP8-Analysis VERSION 3.0.1)

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

### link libs
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")
link_libraries(libResort64c_x64.a)

### add subdirs
add_subdirectory(AnalysisCore)
add_subdirectory(Core)

### compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS}")

### add sort
set(SORTEXE_SOURCE_FILES
    SortExe/LMF_IO.cpp
    SortExe/Main.cpp
    SortExe/SortRun.cpp
    SortExe/SortWrapper.cpp
)
add_executable(sp8sort ${SORTEXE_SOURCE_FILES})
target_link_libraries(sp8sort sp8core)

### add ana
set(ANALYSISEXE_SOURCE_FILES
    AnalysisExe/Main.cpp
    AnalysisExe/AnalysisRun.cpp
)
add_executable(sp8ana ${ANALYSISEXE_SOURCE_FILES})
target_link_libraries(sp8ana anacore sp8core)

### pack
install(
    TARGETS sp8sort sp8ana
    RUNTIME DESTINATION bin
)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "RPM")
include(CPack)

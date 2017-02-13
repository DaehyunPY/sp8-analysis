cmake_minimum_required(VERSION 3.6)
project(SP8-Analysis VERSION 3.0.0)

### gsl 
link_libraries(gsl gslcblas)

### ROOT
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

### rapidjson
include_directories(rapidjson/include)

### pybind
add_subdirectory(pybind11)

### pack
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "RPM")
include(CPack)

### resort
include_directories(resort/include)
link_directories(resort/lib)
link_libraries(libResort64c_x64.a)

include_directories(include)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CFLAGS})
add_subdirectory(Core)
add_subdirectory(SortExe)
add_subdirectory(AnalysisCore)
add_subdirectory(AnalysisExe)
add_subdirectory(accel)

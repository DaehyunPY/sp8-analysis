cmake_minimum_required(VERSION 3.6)
project(accel)

set(SOURCE_FILES accel.cpp)
add_subdirectory(pybind11)
link_libraries(gsl gslcblas)

add_library(accel-cpp STATIC ${SOURCE_FILES})
set_target_properties(accel-cpp
        PROPERTIES OUTPUT_NAME accel)

pybind11_add_module(accel-cpy binder.cpp ${SOURCE_FILES})
set_target_properties(accel-cpy
        PROPERTIES OUTPUT_NAME accel)
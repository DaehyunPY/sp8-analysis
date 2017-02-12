cmake_minimum_required(VERSION 3.6)
project(accel-test)

add_subdirectory(accel)
add_executable(accel-test main.cpp)
target_link_libraries(accel-test accel-cpp)
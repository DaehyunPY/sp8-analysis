cmake_minimum_required(VERSION 3.3)
project(AnalysisCore)

### import JSONHandler
add_subdirectory(JSONHandler)
include_directories(JSONHandler/include)

### main
if((CMAKE_SYSTEM_NAME MATCHES "Linux") OR (CMAKE_SYSTEM_NAME MATCHES "Darwin"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
set(SOURCE_FILES
    AnalysisTools.cpp
    Electron.cpp
    Electrons.cpp
    EquipmentParameters.cpp
    EventDataReader.cpp
    Flag.cpp
    Ion.cpp
    Ions.cpp
    LogWriter.cpp
    Object.cpp
    ObjectFlag.cpp
    ObjectParameters.cpp
    Objects.cpp
    Unit.cpp
    )
add_library(AnalysisCore STATIC ${SOURCE_FILES})
target_link_libraries(AnalysisCore JSONHandler)

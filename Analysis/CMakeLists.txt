cmake_minimum_required(VERSION 3.3)
project(Analysis)

### core setting
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
set(HEADER_FILES
    JSONReader.h
    EventDataReader.h
    Unit.h
    Flag.h
    ObjectFlag.h
    JSONFlag.h
    EventDataFlag.h
    Object.h
    Objects.h
    Ion.h
    Ions.h
    Electron.h
    Electrons.h
    EquipmentParameters.h
    ObjectParameters.h
    IonParameters.h
    ElectronParameters.h
    AnalysisTools.h
    LogWriter.h
    )
set(SOURCE_FILES
    JSONReader.cpp
    EventDataReader.cpp
    Unit.cpp
    Flag.cpp
    ObjectFlag.cpp
    JSONFlag.cpp
    EventDataFlag.cpp
    Object.cpp
    Objects.cpp
    Ion.cpp
    Ions.cpp
    Electron.cpp
    Electrons.cpp
    EquipmentParameters.cpp
    ObjectParameters.cpp
    IonParameters.cpp
    ElectronParameters.cpp
    AnalysisTools.cpp
    LogWriter.cpp
    )

### install library
add_library(AnalysisSharedLib SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(AnalysisStaticLib STATIC ${HEADER_FILES} ${SOURCE_FILES})

### example executable
set(EXAMPLE_FILES Example/Main.cpp)
add_executable(Example ${EXAMPLE_FILES})
# target_link_libraries(Example AnalysisSharedLib)
target_link_libraries(Example AnalysisStaticLib)

cmake_minimum_required(VERSION 3.3)
project(Analysis)

### core setting
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES
    JSONReader.cpp
    JSONReader.h
    EventDataReader.cpp
    EventDataReader.h
    Unit.cpp
    Unit.h
    Flag.cpp
    Flag.h
    ObjectFlag.cpp
    ObjectFlag.h
    JSONFlag.cpp
    JSONFlag.h
    EventDataFlag.cpp
    EventDataFlag.h
    Object.cpp
    Object.h
    Objects.cpp
    Objects.h
    Ion.cpp
    Ion.h
    Ions.cpp
    Ions.h
    Electron.cpp
    Electron.h
    Electrons.cpp
    Electrons.h
    EquipmentParameters.cpp
    EquipmentParameters.h
    ObjectParameters.cpp
    ObjectParameters.h
    IonParameters.cpp
    IonParameters.h
    ElectronParameters.cpp
    ElectronParameters.h
    AnalysisTools.cpp
    AnalysisTools.h
    LogWriter.cpp
    LogWriter.h
    Initialize.cpp
    Initialize.h
    ProcessEvent.cpp
    ProcessEvent.h
    Finalize.cpp Finalize.h
    )

### install library
add_library(AnalysisSharedLib SHARED ${SOURCE_FILES})
add_library(AnalysisStaticLib STATIC ${SOURCE_FILES})

### test executable
set(MAIN_FILE Test/Main.cpp)
add_executable(AnalysisTest ${MAIN_FILE})
# target_link_libraries(AnalysisTest AnalysisSharedLib)
target_link_libraries(AnalysisTest AnalysisStaticLib)


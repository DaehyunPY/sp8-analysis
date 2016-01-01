cmake_minimum_required(VERSION 3.3)
project(Analysis)

### Analysis
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
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
    )
add_library(AnalysisSharedLib SHARED ${SOURCE_FILES})
add_library(AnalysisStaticLib STATIC ${SOURCE_FILES})

### test
set(MAIN_FILE Test/Main.cpp)
add_executable(AnalysisTest ${MAIN_FILE})
target_link_libraries(AnalysisTest AnalysisStaticLib)


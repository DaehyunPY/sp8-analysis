cmake_minimum_required(VERSION 3.3)
project(Analysis)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#add_definitions(-DFOR_COBOLDPC2002)

set(SOURCE_FILES_FOR_ANALYSIS_CORE
    # source for reader
    JSONReader.h
    JSONReader.cpp
    LMFReader.h
    LMFReader.cpp
    # source for unit
    Unit.h
    Unit.cpp
    # source for flag
    Flag.h
    Flag.cpp
    ObjectFlag.h
    ObjectFlag.cpp
    EventDataFlag.h
    EventDataFlag.cpp
    # source for object: ion and electron
    Object.h
    Object.cpp
    Objects.h
    Objects.cpp
    Ion.h
    Ion.cpp
    Ions.h
    Ions.cpp
    Electron.h
    Electron.cpp
    Electrons.h
    Electrons.cpp
    # source for analysis
    EquipmentParameters.h
    EquipmentParameters.cpp
    ObjectParameters.h
    ObjectParameters.cpp
    IonParameters.h
    IonParameters.cpp
    ElectronParameters.h
    ElectronParameters.cpp
    AnalysisTools.h
    AnalysisTools.cpp
    # new sources
    )

set(SOURCE_FILES_FOR_ANALYSIS_EXAMPLE
    Example/Main.cpp
    )

add_executable(EXAMPLE
    ${SOURCE_FILES_FOR_ANALYSIS_EXAMPLE}
    ${SOURCE_FILES_FOR_ANALYSIS_CORE}
    )
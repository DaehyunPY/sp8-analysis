cmake_minimum_required(VERSION 3.3)
project(Analysis)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    JSONReader.cpp JSONReader.h
    LMFReader.cpp LMFReader.h

    Unit.cpp Unit.h
    Flag.cpp Flag.h
    ObjectFlag.cpp ObjectFlag.h
    EventDataFlag.cpp EventDataFlag.h

    Object.cpp Object.h
    Objects.cpp Objects.h
    Ion.cpp Ion.h
    Ions.cpp Ions.h
    Electron.cpp Electron.h
    Electrons.cpp Electrons.h

    EquipmentParameters.cpp EquipmentParameters.h
    ObjectParameters.cpp ObjectParameters.h
    IonParameters.cpp IonParameters.h
    ElectronParameters.cpp ElectronParameters.h
    AnalysisTools.cpp AnalysisTools.h
    )

set(DEBUG_FILES Debug.cpp Debug.h)

add_executable(Debug ${SOURCE_FILES} ${DEBUG_FILES})
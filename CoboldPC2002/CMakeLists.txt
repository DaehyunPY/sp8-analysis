cmake_minimum_required(VERSION 3.3)
project(CoboldPC2002)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MFC_FLAG 2)
include_directories("${PROJECT_SOURCE_DIR}/../include")
add_definitions(-D_AFXDLL)
add_definitions(-DFOR_COBOLDPC2002)

set(SOURCE_FILES_FOR_ANALYSIS_CORE
    ../include/Analysis/JSONReader.h
    ../include/Analysis/JSONReader.cpp
    ../include/Analysis/LMFReader.h
    ../include/Analysis/LMFReader.cpp
    ../include/Analysis/Unit.h
    ../include/Analysis/Unit.cpp
    ../include/Analysis/Flag.h
    ../include/Analysis/Flag.cpp
    ../include/Analysis/ObjectFlag.h
    ../include/Analysis/ObjectFlag.cpp
    ../include/Analysis/EventDataFlag.h
    ../include/Analysis/EventDataFlag.cpp
    ../include/Analysis/Object.h
    ../include/Analysis/Object.cpp
    ../include/Analysis/Objects.h
    ../include/Analysis/Objects.cpp
    ../include/Analysis/Ion.h
    ../include/Analysis/Ion.cpp
    ../include/Analysis/Ions.h
    ../include/Analysis/Ions.cpp
    ../include/Analysis/Electron.h
    ../include/Analysis/Electron.cpp
    ../include/Analysis/Electrons.h
    ../include/Analysis/Electrons.cpp
    ../include/Analysis/EquipmentParameters.h
    ../include/Analysis/EquipmentParameters.cpp
    ../include/Analysis/ObjectParameters.h
    ../include/Analysis/ObjectParameters.cpp
    ../include/Analysis/IonParameters.h
    ../include/Analysis/IonParameters.cpp
    ../include/Analysis/ElectronParameters.h
    ../include/Analysis/ElectronParameters.cpp
    ../include/Analysis/AnalysisTools.h
    ../include/Analysis/AnalysisTools.cpp
    )

set(SOURCE_FILES_FOR_COBOLD2002
    Res/CDan.rc2
    AnalysisDAN.cpp
    CDan.cpp
    CDan.h
    CDan.rc
    CDAQ.h
    CheckProcess.H
    CoboldDll.h
    DanUtilities.cpp
    DanUtilities.h
    LMInfo.h
    Resource.h
    SoftwareDependent.cpp
    StdAfx.cpp
    StdAfx.h
    )

add_library(CDan SHARED ${SOURCE_FILES_FOR_COBOLD2002} ${SOURCE_FILES_FOR_ANALYSIS_CORE})
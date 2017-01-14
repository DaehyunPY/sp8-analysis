cmake_minimum_required(VERSION 3.6)
project(AnalysisCore)

set(SOURCE_FILES
    AnalysisTools.cpp
    EquipmentParameters.cpp
    EventDataReader.cpp
    LogWriter.cpp
    Object.cpp
    ObjectFlag.cpp
    ObjectParameters.cpp
    Objects.cpp
    )
add_library(AnalysisCore SHARED ${SOURCE_FILES})

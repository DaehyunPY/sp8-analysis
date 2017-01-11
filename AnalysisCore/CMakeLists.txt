cmake_minimum_required(VERSION 2.8)
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
add_library(AnalysisCore STATIC ${SOURCE_FILES})

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
add_library(anacore STATIC ${SOURCE_FILES})

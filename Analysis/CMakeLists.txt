cmake_minimum_required(VERSION 3.3)
project(Analysis)

### core setting
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_definitions(-D_AFXDLL)
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
set(HEADER_FILES
    JSONReader.h
    LMFReader.h
    Unit.h
    Flag.h
    ObjectFlag.h
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
    )
set(SOURCE_FILES
    JSONReader.cpp
    LMFReader.cpp
    Unit.cpp
    Flag.cpp
    ObjectFlag.cpp
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
    )

### install library
add_library(AnalysisSharedLib SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(AnalysisStaticLib STATIC ${HEADER_FILES} ${SOURCE_FILES})
install(TARGETS AnalysisSharedLib AnalysisStaticLib
    LIBRARY DESTINATION "lib" RENAME ${PROJECT_NAME}
    ARCHIVE DESTINATION "lib" RENAME ${PROJECT_NAME}
    )
install(FILES ${HEADER_FILES} DESTINATION "include/${PROJECT_NAME}")

### example executable
set(EXAMPLE_FILES Example/Main.cpp)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_executable(AnalysisExample WIN32 ${EXAMPLE_FILES})
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_executable(AnalysisExample ${EXAMPLE_FILES})
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
# target_link_libraries(AnalysisExample AnalysisSharedLib)
target_link_libraries(AnalysisExample AnalysisStaticLib)

cmake_minimum_required(VERSION 3.3)
project(BL17_Analysis)

### sub directory
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_definitions(-DFOR_COBOLDPC2002)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_subdirectory(Analysis)

### for windows
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  ### core setting
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(CMAKE_MFC_FLAG 2)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
  add_definitions(-D_AFXDLL)
  include_directories("${PROJECT_SOURCE_DIR}")
  set(SOURCE_FILES
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
      SoftwareDependent.h
      StdAfx.cpp
      StdAfx.h
      )

  ### install library
  add_library(CDan SHARED WIN32 ${SOURCE_FILES})
  install(TAGETS CDan LIBRARY DESTINATION "lib")
  target_link_libraries(CDan AnalysisSharedLib)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

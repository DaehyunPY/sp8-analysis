cmake_minimum_required(VERSION 3.3)
project(BL17_Analysis)

### analysis library 
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  include_directories("${PROJECT_SOURCE_DIR}")
  add_definitions(-DFOR_COBOLDPC2002)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_subdirectory(Analysis)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  ### core setting
  add_definitions(-D_AFXDLL)
  set(CMAKE_MFC_FLAG 2)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
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
  add_library(CDan SHARED ${SOURCE_FILES})
  install(TARGETS CDan 
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    )
  # target_link_libraries(CDan AnalysisSharedLib)
  target_link_libraries(CDan AnalysisStaticLib)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

cmake_minimum_required(VERSION 3.3)
project(BL17_Analysis)

### analysis library 
add_subdirectory(Analysis)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  ### core setting
  add_definitions(-D_AFXDLL)
  add_definitions(-D_USRDLL)
  add_definitions(-DCDAN_EXPORTS)
  set(CMAKE_MFC_FLAG 2)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
  set(SOURCE_FILES
      Res/CDan.rc2
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

  ### install library
  add_library(DAn SHARED ${SOURCE_FILES})
  target_link_libraries(DAn AnalysisStaticLib)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

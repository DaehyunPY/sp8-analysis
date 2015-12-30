cmake_minimum_required(VERSION 3.3)
project(BL17Analysis)

### add subdirectory
add_subdirectory(Analysis)

### BL17Analysis
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES
    Run.cpp
    Run.h
    )
add_library(BL17Analysis STATIC ${SOURCE_FILES})
target_link_libraries(BL17Analysis AnalysisStaticLib)

### DAn
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_definitions(-D_AFXDLL)
  add_definitions(-D_USRDLL)
  add_definitions(-DCDAN_EXPORTS)
  set(CMAKE_MFC_FLAG 2)
  set(COBOLDPC_FILES
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
  add_library(DAn SHARED ${COBOLDPC_FILES})
  target_link_libraries(DAn BL17Analysis)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

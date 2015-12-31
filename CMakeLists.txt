cmake_minimum_required(VERSION 3.3)
project(BL17Analysis)

### add subdirectory
add_subdirectory(Analysis)

### BL17Analysis
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -pthread -m64")
  link_libraries(pthread m dl)
  include_directories("/usr/local/include/root")
  link_directories("/usr/local/lib/root")
  link_libraries(
      Core
      RIO
      Net
      Hist
      Graf
      Graf3d
      Gpad
      Tree
      Rint
      Postscript
      Matrix
      Physics
      MathCore
      Thread
  )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -pthread -m64")
  link_libraries(pthread m dl)
  include_directories("/usr/local/include/root")
  link_directories("/usr/local/lib/root")
  link_libraries(
      Core
      RIO
      Net
      Hist
      Graf
      Graf3d
      Gpad
      Tree
      Rint
      Postscript
      Matrix
      Physics
      MathCore
      Thread
  )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(ROOTSYS "C:/root/")
  include_directories("${ROOTSYS}/include")
  link_directories("${ROOTSYS}/lib")
  link_libraries(
      libCore.lib
      libRIO.lib
      libNet.lib
      libHist.lib
      libGraf.lib
      libGraf3d.lib
      libGpad.lib
      libTree.lib
      libRint.lib
      libPostscript.lib
      libMatrix.lib
      libPhysics.lib
      libMathCore.lib
      libThread.lib
  )
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(SOURCE_FILES
    RUN.cpp
    RUN.h
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

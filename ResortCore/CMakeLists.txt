cmake_minimum_required(VERSION 3.3)
project(Resort)

### projects using ROOT 
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

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_definitions(-DWIN32)
  add_definitions(-D_WINDOWS)
  add_definitions(-D_AFXDLL)
  add_definitions(-D_USRDLL)
  add_definitions(-DCDAN_EXPORTS)
  link_directories("./")
  link_libraries(
      Cobold.lib
      resort64.lib
      UserSP.lib
  )
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
      LMFPreEventData.h
      LMInfo.h
      resort64.Import.h
      Resource.h
      StandardDAN.cpp
      StdAfx.cpp
      StdAfx.h
      UserSP.h
      )
  add_library(DAn_Resort SHARED ${COBOLDPC_FILES})
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

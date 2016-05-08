cmake_minimum_required(VERSION 3.3)
project(Analysis)

### Flag for debugging 
if (CMAKE_BUILD_TYPE EQUAL "Debug")
  add_definitions(-DANALYSIS_DEBUG_BUILD)
endif ()

### Core
add_subdirectory(Core)

### ROOT
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  link_libraries(m dl)
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
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  link_libraries(m dl)
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
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
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
endif ()

### Analysis
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -m64")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -m64")
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()
set(SOURCE_FILES
    Main.cpp
    Run.cpp
    Run.h
    OutputFlag.cpp
    OutputFlag.h
    OutputHist.cpp
    OutputHist.h
    )
add_executable(AnalysisExe ${SOURCE_FILES})
target_link_libraries(AnalysisExe AnalysisCoreStaticLib)

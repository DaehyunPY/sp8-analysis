cmake_minimum_required (VERSION 3.3)
project (Analysis)

if (CMAKE_BUILD_TYPE EQUAL "Debug")
  add_definitions(-DANALYSIS_DEBUG_BUILD)
endif (CMAKE_BUILD_TYPE EQUAL "Debug")

### Add core library.
add_subdirectory (Core)

### Link ROOT libraries.
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  link_libraries (pthread m dl)
  include_directories ("/usr/local/include/root")
  link_directories ("/usr/local/lib/root")
  link_libraries (
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
  link_libraries (pthread m dl)
  include_directories ("/usr/local/include/root")
  link_directories ("/usr/local/lib/root")
  link_libraries (
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
  set (ROOTSYS "C:/root/")
  include_directories ("${ROOTSYS}/include")
  link_directories ("${ROOTSYS}/lib")
  link_libraries (
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
endif (CMAKE_SYSTEM_NAME MATCHES "Linux")

### Set flags.
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -pthread -m64")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -pthread -m64")
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
endif (CMAKE_SYSTEM_NAME MATCHES "Linux")

### Set source files.
set (SOURCE_FILES
    Main.cpp
    Run.cpp
    Run.h
    OutputFlag.cpp
    OutputFlag.h
    MyHistos.cpp
    MyHistos.h
    )

### Add executable.
add_executable (AnalysisExe ${SOURCE_FILES})
target_link_libraries (AnalysisExe AnalysisCoreStaticLib)

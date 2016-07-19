cmake_minimum_required(VERSION 3.3)
project(AnalysisExe)

### import ROOT
set(ROOTSYS "$ENV{ROOTSYS}")
if(ROOTSYS) 
    message(STATUS "ROOTSYS is... ${ROOTSYS}")
else()
    message(SEND_ERROR "ROOTSYS is not defined")
endif()
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    include_directories("${ROOTSYS}/include")
    link_directories("${ROOTSYS}/lib")
    link_libraries(m dl)
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
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    include_directories("${ROOTSYS}/include/root")
    link_directories("${ROOTSYS}/lib/root")
    link_libraries(m dl)
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
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
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
endif()

### main
if((CMAKE_SYSTEM_NAME MATCHES "Linux") OR (CMAKE_SYSTEM_NAME MATCHES "Darwin"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
set(SOURCE_FILES
    Main.cpp
    Run.cpp
    OutputFlag.cpp
    OutputHist.cpp
    )
add_executable(AnalysisExe ${SOURCE_FILES})
target_link_libraries(AnalysisExe ../AnalysisCore/AnalysisCore)

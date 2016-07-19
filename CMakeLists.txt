cmake_minimum_required(VERSION 3.3)
project(Analysis)

### add sub lib
add_subdirectory(AnalysisCore)
add_subdirectory(JSONHandler)
add_subdirectory(Protocols)
# add_subdirectory(ResortCore)

### link ROOT
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

### compiler
if((CMAKE_SYSTEM_NAME MATCHES "Linux") OR (CMAKE_SYSTEM_NAME MATCHES "Darwin"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

### AnalysisExe
set(ANALYSISEXE_SOURCE_FILES
    AnalysisExe/Main.cpp
    AnalysisExe/Run.cpp
    AnalysisExe/OutputFlag.cpp
    AnalysisExe/OutputHist.cpp
    )
add_executable(AnalysisExe ${ANALYSISEXE_SOURCE_FILES})
target_link_libraries(AnalysisExe AnalysisCore JSONHandler Protocols)

### ResortExe
if((CMAKE_SYSTEM_NAME MATCHES "Linux") OR (CMAKE_SYSTEM_NAME MATCHES "Windows"))
    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        # link_libraries(libResort64c_x86.a)
        link_libraries(libResort64c_x64.a)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
        link_libraries(resort64c_VS2010_x86.lib)
    endif()
    set(RESORTEXE_SOURCE_FILES
        ResortExe/sort_with_root.cpp
        ResortExe/rootstuff.cpp
        ResortExe/LMF_IO.cpp
        ResortExe/Main.cpp
        )
    add_executable(ResortExe ${RESORTEXE_SOURCE_FILES})
    target_link_libraries(ResortExe JSONHandler Protocols ResortCore)
endif()

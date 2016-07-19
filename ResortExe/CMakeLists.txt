cmake_minimum_required(VERSION 3.3)
project(ResortExe)

### Note
# Resort64c doesn't support root6 yet

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

### import Resort64c
if((CMAKE_SYSTEM_NAME MATCHES "Linux") OR (CMAKE_SYSTEM_NAME MATCHES "Darwin"))
    # link_libraries(libResort64c_x86.a)
    link_libraries(libResort64c_x64.a)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    link_libraries(resort64c_VS2010_x86.lib)
endif()

### main
set(SOURCE_FILES
    sort_with_root.cpp
    rootstuff.cpp
    LMF_IO.cpp
    )
add_executable(ResortExe ${SOURCE_FILES})

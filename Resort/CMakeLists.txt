cmake_minimum_required(VERSION 3.3)
project(Sort)

message(STATUS "CMake system is... ${CMAKE_SYSTEM_NAME}")


### link ROOT: `resort` doesn't support root6 yet
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
        Hist
        Gpad
        Tree
        Rint
        )
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    include_directories("${ROOTSYS}/include/root")
    link_directories("${ROOTSYS}/lib/root")
    link_libraries(m dl)
    link_libraries(
        Core
        RIO
        Hist
        Gpad
        Tree
        Rint
        )
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    include_directories("${ROOTSYS}/include")
    link_directories("${ROOTSYS}/lib")
    link_libraries(
        libCore.lib
        libRIO.lib
        libHist.lib
        libGpad.lib
        libTree.lib
        libRint.lib
        )
endif()


### link resort 
link_directories("./")
if((CMAKE_SYSTEM_NAME MATCHES "Linux") OR (CMAKE_SYSTEM_NAME MATCHES "Darwin"))
    # link_libraries(libResort64c_x86.a)
    link_libraries(libResort64c_x64.a)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    link_libraries(resort64c_VS2010_x86.lib)
endif()


### project source
set(SOURCE_FILES
    sort_with_root.cpp
    rootstuff.cpp
    LMF_IO.cpp
    )
add_executable(SortExe ${SOURCE_FILES})

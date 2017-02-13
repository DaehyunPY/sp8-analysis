set(SOURCE_FILES
    LMF_IO.cpp
    Main.cpp
    SortRun.cpp
    SortWrapper.cpp
)
add_executable(sp8sort ${SOURCE_FILES})
target_link_libraries(sp8sort sp8core)
install(
    TARGETS sp8sort
    RUNTIME DESTINATION bin
)

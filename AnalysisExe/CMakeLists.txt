set(SOURCE_FILES
    Main.cpp
    AnalysisRun.cpp
)
add_executable(sp8ana ${SOURCE_FILES})
target_link_libraries(sp8ana anacore sp8core)
install(
    TARGETS sp8ana
    RUNTIME DESTINATION bin
)

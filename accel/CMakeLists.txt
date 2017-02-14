set(SOURCE_FILES 
    accel.cpp
)
add_library(accel STATIC ${SOURCE_FILES})
pybind11_add_module(accel-py binder.cpp ${SOURCE_FILES})
set_target_properties(accel-py PROPERTIES OUTPUT_NAME accel)
install(TARGETS accel-py
    LIBRARY DESTINATION lib
)

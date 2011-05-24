include_directories (${Coal_SOURCE_DIR}/include)

message(STATUS "source is ${Coal_SOURCE_DIR}")
link_directories(${Coal_BINARY_DIR}/src)

add_executable(basic basic.c)

set_source_files_properties(basic.c
        PROPERTIES COMPILE_FLAGS -Wno-format)

target_link_libraries(basic OpenCL)

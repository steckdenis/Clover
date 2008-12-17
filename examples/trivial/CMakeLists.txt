include_directories (${Clover_SOURCE_DIR}/include)

message(STATUS "source is ${Clover_SOURCE_DIR}")
link_directories (${Clover_BINARY_DIR}/src)

add_executable (basic basic.c)

target_link_libraries (basic OpenCL)

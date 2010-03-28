include_directories (${Coal_SOURCE_DIR}/include)

message(STATUS "source is ${Coal_SOURCE_DIR}")
link_directories(${Coal_BINARY_DIR}/src)

add_executable(basic basic.c)

target_link_libraries(basic OpenCL)

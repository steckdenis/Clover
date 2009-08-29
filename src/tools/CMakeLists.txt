include_directories (${Clover_SOURCE_DIR}/include
                    ${Clover_SOURCE_DIR}/src/tools
                    ${Clover_SOURCE_DIR}/src/compiler)

message(STATUS "XXXXXXX source is ${Clover_SOURCE_DIR}")
link_directories(${Clover_BINARY_DIR}/src)

add_executable(clcompiler clcompiler.cpp ${Clover_SOURCE_DIR}/src/compiler)

target_link_libraries(clcompiler OpenCL)

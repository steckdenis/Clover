include_directories (${Coal_SOURCE_DIR}/include
                    ${Coal_SOURCE_DIR}/src/tools
                    ${Coal_SOURCE_DIR}/src/compiler)

link_directories(${Coal_BINARY_DIR}/src)

add_executable(clcompiler clcompiler.cpp ${Coal_SOURCE_DIR}/src/compiler)

target_link_libraries(clcompiler OpenCL)

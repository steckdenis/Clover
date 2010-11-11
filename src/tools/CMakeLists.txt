include_directories (${Coal_SOURCE_DIR}/include
                    ${Coal_SOURCE_DIR}/src/tools
                    ${Coal_SOURCE_DIR}/src/compiler
                    ${LLVM_INCLUDE_DIR}
                    ${CLANG_INCLUDE_DIRS})

set(COMPILER_SRC_FILES
    clcompiler.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

link_directories(${Coal_BINARY_DIR}/src)

set_source_files_properties( ${COMPILER_SRC_FILES}
        PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS})

add_executable(clcompiler ${COMPILER_SRC_FILES} ${Coal_SOURCE_DIR}/src/compiler)

target_link_libraries(clcompiler OpenCL ${CLANG_LIBS}
                             ${LLVM_LIBS_CORE})

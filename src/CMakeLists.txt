include_directories (${Clover_SOURCE_DIR}/include
                    ${Clover_SOURCE_DIR}
                    ${LLVM_INCLUDE_DIR}
                    ${GALLIUM_INCLUDE_DIRS}
                    ${CLANG_INCLUDE_DIRS}
                    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

set(CLOVER_SRC_FILES
    core/api_command.cpp  core/api_device.cpp
    core/api_event.cpp  core/api_kernel.cpp
    core/api_platform.cpp   core/api_program.cpp
    core/device.cpp core/api_context.cpp
    core/api_enqueue.cpp  core/api_flush.cpp
    core/api_memory.cpp  core/api_profiling.cpp
    core/api_sampler.cpp core/api_gl.cpp
    compiler/compiler.cpp
    ../cpuwinsys/cpuwinsys.c)

add_library(OpenCL SHARED ${CLOVER_SRC_FILES})

SET(LIBRARY_OUTPUT_PATH ${Clover_BINARY_DIR}/lib)

SET_TARGET_PROPERTIES(OpenCL PROPERTIES
  VERSION ${${PROJECT_NAME}_VERSION}
  SOVERSION ${${PROJECT_NAME}_SOVERSION} )
#message(STATUS "some project ${${PROJECT_NAME}_VERSION}")

set_source_files_properties( ${CLOVER_SRC_FILES}
        PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS})
set_target_properties(OpenCL
                      PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS})


TARGET_LINK_LIBRARIES(OpenCL
        ${CLANG_LIBS}
        ${LLVM_LIBS_CORE}
        ${GALLIUM_LIBS}
        )

add_subdirectory(tools)

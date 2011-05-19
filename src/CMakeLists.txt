include_directories (${Coal_SOURCE_DIR}/include
                    ${Coal_SOURCE_DIR}/src
                    ${LLVM_INCLUDE_DIR}
                    ${GALLIUM_INCLUDE_DIRS}
                    ${CLANG_INCLUDE_DIRS}
                    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

set(COAL_SRC_FILES
    api/api_command.cpp
    api/api_device.cpp
    api/api_event.cpp
    api/api_kernel.cpp
    api/api_platform.cpp
    api/api_program.cpp
    api/api_context.cpp
    api/api_enqueue.cpp
    api/api_flush.cpp
    api/api_memory.cpp
    api/api_profiling.cpp
    api/api_sampler.cpp
    api/api_gl.cpp
    
    core/cpudevice.cpp
)

configure_file(builtins/coal-internal.h.in
              ${CMAKE_CURRENT_BINARY_DIR}/builtins/coal-internal.h )

add_library(OpenCL SHARED ${COAL_SRC_FILES})

SET(LIBRARY_OUTPUT_PATH ${Coal_BINARY_DIR}/lib)

SET_TARGET_PROPERTIES(OpenCL PROPERTIES
  VERSION ${${PROJECT_NAME}_VERSION}
  SOVERSION ${${PROJECT_NAME}_SOVERSION} )
#message(STATUS "some project ${${PROJECT_NAME}_VERSION}")

set_source_files_properties( ${COAL_SRC_FILES}
        PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS})

set_target_properties(OpenCL PROPERTIES
  LINK_FLAGS ${LLVM_LDFLAGS}
  LINK_INTERFACE_LIBRARIES "")


TARGET_LINK_LIBRARIES(OpenCL
        ${CLANG_LIBS}
        ${LLVM_LIBS_CORE}
#        ${GALLIUM_LIBS}
        )

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/src/builtins/coal-internal.h  DESTINATION  ${INCLUDE_INSTALL_DIR}/coal)

#add_subdirectory(tools)

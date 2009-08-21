include_directories (${Clover_SOURCE_DIR}/include
                    ${Clover_SOURCE_DIR}
                    ${LLVM_INCLUDE_DIR}
                    ${GALLIUM}/include
                    ${GALLIUM}/src/gallium/include
                    ${GALLIUM}/src/gallium/auxiliary
                    ${GALLIUM}/src/gallium/drivers
                    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")

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

find_library(CLANG_CODEGEN_LIB clangCodeGen)
if (CLANG_CODEGEN_LIB)
   target_link_libraries(OpenCL ${CLANG_CODEGEN_LIB})
endif(CLANG_CODEGEN_LIB)

find_library(CLANG_ANALYSIS_LIB clangAnalysis)
if (CLANG_ANALYSIS_LIB)
   target_link_libraries(OpenCL ${CLANG_ANALYSIS_LIB})
endif(CLANG_ANALYSIS_LIB)

find_library(CLANG_REWRITE_LIB clangRewrite)
if (CLANG_REWRITE_LIB)
   target_link_libraries(OpenCL ${CLANG_REWRITE_LIB})
endif(CLANG_REWRITE_LIB)

find_library(CLANG_SEMA_LIB clangSema)
if (CLANG_SEMA_LIB)
   target_link_libraries(OpenCL ${CLANG_SEMA_LIB})
endif(CLANG_SEMA_LIB)

find_library(CLANG_DRIVER_LIB clangDriver)
if (CLANG_DRIVER_LIB)
   target_link_libraries(OpenCL ${CLANG_DRIVER_LIB})
endif(CLANG_DRIVER_LIB)

find_library(CLANG_AST_LIB clangAST)
if (CLANG_AST_LIB)
   target_link_libraries(OpenCL ${CLANG_AST_LIB})
endif(CLANG_AST_LIB)

find_library(CLANG_PARSE_LIB clangParse)
if (CLANG_PARSE_LIB)
   target_link_libraries(OpenCL ${CLANG_PARSE_LIB})
endif(CLANG_PARSE_LIB)

find_library(CLANG_LEX_LIB clangLex)
if (CLANG_LEX_LIB)
   target_link_libraries(OpenCL ${CLANG_LEX_LIB})
endif(CLANG_LEX_LIB)

find_library(CLANG_BASIC_LIB clangBasic)
if (CLANG_BASIC_LIB)
   target_link_libraries(OpenCL ${CLANG_BASIC_LIB})
endif(CLANG_BASIC_LIB)

find_library(CLANG_FRONTEND_LIB clangFrontend)
if (CLANG_FRONTEND_LIB)
   target_link_libraries(OpenCL ${CLANG_FRONTEND_LIB})
endif(CLANG_FRONTEND_LIB)


TARGET_LINK_LIBRARIES(OpenCL
        ${GALLIUM}/src/gallium/auxiliary/pipebuffer/libpipebuffer.a
        ${GALLIUM}/src/gallium/auxiliary/sct/libsct.a
        ${GALLIUM}/src/gallium/auxiliary/draw/libdraw.a
        ${GALLIUM}/src/gallium/auxiliary/rtasm/librtasm.a
        ${GALLIUM}/src/gallium/auxiliary/translate/libtranslate.a
        ${GALLIUM}/src/gallium/auxiliary/cso_cache/libcso_cache.a
        ${GALLIUM}/src/gallium/auxiliary/tgsi/libtgsi.a
        ${GALLIUM}/src/gallium/drivers/softpipe/libsoftpipe.a
        ${GALLIUM}/src/gallium/auxiliary/util/libutil.a
        ${LLVM_LIBS_CORE}
        )

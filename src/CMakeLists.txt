include_directories (${Clover_SOURCE_DIR}/include
                    ${Clover_SOURCE_DIR}
                    ${GALLIUM}/include
                    ${GALLIUM}/src/gallium/include
                    ${GALLIUM}/src/gallium/auxiliary
                    ${GALLIUM}/src/gallium/drivers
                    )

add_library(OpenCL SHARED
        core/api_command.cpp  core/api_device.cpp
        core/api_event.cpp  core/api_kernel.cpp
        core/api_platform.cpp   core/api_program.cpp
        core/device.cpp core/api_context.cpp
        core/api_enqueue.cpp  core/api_flush.cpp
        core/api_memory.cpp  core/api_profiling.cpp
        core/api_sampler.cpp core/api_gl.cpp
        ../cpuwinsys/cpuwinsys.c)

SET(LIBRARY_OUTPUT_PATH ${Clover_BINARY_DIR}/lib)

SET_TARGET_PROPERTIES(OpenCL PROPERTIES
  VERSION ${${PROJECT_NAME}_VERSION}
  SOVERSION ${${PROJECT_NAME}_SOVERSION} )
#message(STATUS "some project ${${PROJECT_NAME}_VERSION}")

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
        )


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


find_library(LLVM_BITREADER_LIB LLVMBitReader)
if (LLVM_BITREADER_LIB)
   target_link_libraries(OpenCL ${LLVM_BITREADER_LIB})
endif(LLVM_BITREADER_LIB)


find_library(LLVM_BITWRITER_LIB LLVMBitWriter)
if (LLVM_BITWRITER_LIB)
   target_link_libraries(OpenCL ${LLVM_BITWRITER_LIB})
endif(LLVM_BITWRITER_LIB)


find_library(LLVM_CODEGEN_LIB LLVMCodeGen)
if (LLVM_CODEGEN_LIB)
   target_link_libraries(OpenCL ${LLVM_CODEGEN_LIB})
endif(LLVM_CODEGEN_LIB)

find_library(LLVM_IPO_LIB LLVMipo)
if (LLVM_IPO_LIB)
   target_link_libraries(OpenCL ${LLVM_IPO_LIB})
endif(LLVM_IPO_LIB)

find_library(LLVM_SELECTIONDAG_LIB LLVMSelectionDAG)
if (LLVM_SELECTIONDAG_LIB)
   target_link_libraries(OpenCL ${LLVM_SELECTIONDAG_LIB})
endif(LLVM_SELECTIONDAG_LIB)

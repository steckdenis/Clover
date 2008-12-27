include_directories (${Clover_SOURCE_DIR}/include
                    ${Clover_SOURCE_DIR}
                    ${GALLIUM}/include
                    ${GALLIUM}/src/gallium/include
                    ${GALLIUM}/src/gallium/auxiliary
                    ${GALLIUM}/src/gallium/drivers
                    )

add_library(OpenCL SHARED
        api_command.cpp  api_device.cpp
        api_event.cpp  api_kernel.cpp
        api_platform.cpp   api_program.cpp
        device.cpp api_context.cpp
        api_enqueue.cpp  api_flush.cpp
        api_memory.cpp  api_profiling.cpp
        api_sampler.cpp ../cpuwinsys/cpuwinsys.c)

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
        ${GALLIUM}/src/gallium/auxiliary/util/libutil.a
        ${GALLIUM}/src/gallium/drivers/softpipe/libsoftpipe.a
        )

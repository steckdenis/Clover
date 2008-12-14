#include "device.h"

#include "OpenCL/cl.h"
#include "OpenCL/cl_platform.h"

#include "pipe/p_screen.h"
#include "pipe/p_format.h"
#include "pipe/p_winsys.h"
#include "util/u_memory.h"

#include "cpuwinsys/cpuwinsys.h"
#include "softpipe/sp_winsys.h"


Device * Device::create(cl_uint type)
{
   switch(type) {
   case CL_DEVICE_TYPE_CPU: {
      struct pipe_winsys *ws = cpu_winsys();
      struct pipe_screen *screen =
         softpipe_create_screen(ws);
      return new Device(CL_DEVICE_TYPE_CPU, ws, screen);
   }
      break;
   case CL_DEVICE_TYPE_GPU:
      break;
   case CL_DEVICE_TYPE_ACCELERATOR:
#ifdef GALLIUM_CELL
    if (!getenv("GALLIUM_NOCELL")) {
        struct cell_winsys *cws = cell_get_winsys(pixelformat);
        struct pipe_screen *screen = cell_create_screen(pws);

        pipe = cell_create_context(screen, cws);
    }
#endif
      break;
   }
   return 0;
}

cl_int Device::info(cl_device_info opcode,
                    size_t paramValueSize,
                    void * paramValue,
                    size_t * paramValueSizeRet) const
{
   switch (opcode) {
   case CL_DEVICE_TYPE:
      ((cl_int*)paramValue)[0] = type();
      break;
   case CL_DEVICE_VENDOR_ID:
      break;
   case CL_DEVICE_MAX_COMPUTE_UNITS:
      break;
   case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:
      break;
   case CL_DEVICE_MAX_WORK_GROUP_SIZE:
      break;
   case CL_DEVICE_MAX_WORK_ITEM_SIZES:
      break;
   case CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR:
      break;
   case CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT:
      break;
   case CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT:
      break;
   case CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG:
      break;
   case CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT:
      break;
   case CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE:
      break;
   case CL_DEVICE_MAX_CLOCK_FREQUENCY:
      break;
   case CL_DEVICE_ADDRESS_BITS:
      break;
   case CL_DEVICE_MAX_READ_IMAGE_ARGS:
      break;
   case CL_DEVICE_MAX_WRITE_IMAGE_ARGS:
      break;
   case CL_DEVICE_MAX_MEM_ALLOC_SIZE:
      break;
   case CL_DEVICE_IMAGE2D_MAX_WIDTH:
      break;
   case CL_DEVICE_IMAGE2D_MAX_HEIGHT:
      break;
   case CL_DEVICE_IMAGE3D_MAX_WIDTH:
      break;
   case CL_DEVICE_IMAGE3D_MAX_HEIGHT:
      break;
   case CL_DEVICE_IMAGE3D_MAX_DEPTH:
      break;
   case CL_DEVICE_IMAGE_SUPPORT:
      break;
   case CL_DEVICE_MAX_PARAMETER_SIZE:
      break;
   case CL_DEVICE_MAX_SAMPLERS:
      break;
   case CL_DEVICE_MEM_BASE_ADDR_ALIGN:
      break;
   case CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE:
      break;
   case CL_DEVICE_SINGLE_FP_CONFIG:
      break;
   case CL_DEVICE_GLOBAL_MEM_CACHE_TYPE:
      break;
   case CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE:
      break;
   case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE:
      break;
   case CL_DEVICE_GLOBAL_MEM_SIZE:
      break;
   case CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:
      break;
   case CL_DEVICE_MAX_CONSTANT_ARGS:
      break;
   case CL_DEVICE_LOCAL_MEM_TYPE:
      break;
   case CL_DEVICE_LOCAL_MEM_SIZE:
      break;
   case CL_DEVICE_ERROR_CORRECTION_SUPPORT:
      break;
   case CL_DEVICE_PROFILING_TIMER_RESOLUTION:
      break;
   case CL_DEVICE_ENDIAN_LITTLE:
      break;
   case CL_DEVICE_AVAILABLE:
      break;
   case CL_DEVICE_COMPILER_AVAILABLE:
      break;
   case CL_DEVICE_EXECUTION_CAPABILITIES:
      break;
   case CL_DEVICE_QUEUE_PROPERTIES:
      break;
   case CL_DEVICE_NAME:
      break;
   case CL_DEVICE_VENDOR:
      break;
   case CL_DRIVER_VERSION:
      break;
   case CL_DEVICE_PROFILE:
      break;
   case CL_DEVICE_VERSION:
      break;
   case CL_DEVICE_EXTENSIONS:
      break;

   default:
      return CL_INVALID_VALUE;
      break;
   }

   return CL_SUCCESS;
}

Device::Device(cl_uint type, struct pipe_winsys *ws,
               struct pipe_screen *screen)
   : m_winsys(ws), m_screen(screen)
{
   m_info.type = type;
   fillInfo();
}

void Device::fillInfo()
{
}

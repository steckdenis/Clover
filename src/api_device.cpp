#include <OpenCL/cl.h>
#include <OpenCL/cl_platform.h>

#include "device.h"
#include "cpuwinsys/cpuwinsys.h"


#include "pipe/p_screen.h"
#include "pipe/p_format.h"
#include "pipe/p_winsys.h"
#include "util/u_memory.h"

#include "softpipe/sp_winsys.h"

// Device APIs

static void
create_gpu_device(cl_device_id *   devices,
                  cl_uint *        num_devices,
                  cl_uint          num_entries)
{
}

static void
create_cpu_device(cl_device_id *   devices,
                  cl_uint *        num_devices,
                  cl_uint          num_entries)
{
   struct pipe_winsys *pws = cpu_winsys();
   struct pipe_screen *screen = softpipe_create_screen(pws);
   struct _cl_device_id *device;

   device = CALLOC_STRUCT(_cl_device_id);
   device->screen = screen;
   device->winsys = pws;
   device->type = CL_DEVICE_TYPE_CPU;

   devices[0] = device;
   *num_devices = 1;
}

static void
create_accel_device(cl_device_id *   devices,
                    cl_uint *        num_devices,
                    cl_uint          num_entries)
{
#ifdef GALLIUM_CELL
    if (!getenv("GALLIUM_NOCELL")) {
        struct cell_winsys *cws = cell_get_winsys(pixelformat);
        struct pipe_screen *screen = cell_create_screen(pws);

        pipe = cell_create_context(screen, cws);
    }
#endif
}


cl_int
clGetDeviceIDs(cl_device_type   device_type,
               cl_uint          num_entries,
               cl_device_id *   devices,
               cl_uint *        num_devices)
{
    cl_bool gpu, cpu, accelerator;
    cl_uint original_num_entries = num_entries;

    gpu = (device_type & CL_DEVICE_TYPE_DEFAULT) ||
          (device_type & CL_DEVICE_TYPE_GPU) ||
          (device_type & CL_DEVICE_TYPE_ALL);

    cpu = (device_type & CL_DEVICE_TYPE_CPU) ||
          (device_type & CL_DEVICE_TYPE_ALL);

    accelerator = (device_type & CL_DEVICE_TYPE_ACCELERATOR) ||
                  (device_type & CL_DEVICE_TYPE_ALL);

    if (!gpu && !cpu && !accelerator)
        return CL_INVALID_DEVICE_TYPE;

    if ((!num_entries && devices) || (!num_devices && !devices))
        return CL_INVALID_VALUE;

    if (gpu && num_entries > 0) {
        cl_uint num_gpus = 0;
        create_gpu_device(devices, &num_gpus, num_entries);
        num_entries -= num_gpus;
        *num_devices += num_gpus;
    }

    if (cpu && num_entries > 0) {
        cl_uint num_cpus = 0;
        create_cpu_device(devices, &num_cpus, num_entries);
        num_entries -= num_cpus;
        *num_devices += num_cpus;
    }

    if (accelerator && num_entries) {
        cl_uint num_accels = 0;
        create_accel_device(devices, &num_accels, num_entries);
        num_entries -= num_accels;
        *num_devices += num_accels;
    }

    if (original_num_entries == num_entries)
        return CL_DEVICE_NOT_FOUND;

    return CL_SUCCESS;
}

cl_int
clGetDeviceInfo(cl_device_id    device,
                cl_device_info  opcode,
                size_t          param_value_size,
                void *          param_value,
                size_t *        param_value_size_ret)
{
   if (!device)
      return CL_INVALID_DEVICE;

   switch(opcode) {
   case CL_DEVICE_TYPE: {
      ((cl_int*)param_value)[0] = device->type;
   }
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
   case CL_DEVICE_MAX_DATA_TYPE_ALIGN_SIZE:
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
   }
   return CL_SUCCESS;
}

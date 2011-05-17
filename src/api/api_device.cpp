#include "CL/cl.h"

cl_int
clGetDeviceIDs(cl_platform_id   platform,
               cl_device_type   device_type,
               cl_uint          num_entries,
               cl_device_id *   devices,
               cl_uint *        num_devices)
{
    return 0;
}

cl_int
clGetDeviceInfo(cl_device_id    device,
                cl_device_info  opcode,
                size_t          param_value_size,
                void *          param_value,
                size_t *        param_value_size_ret)
{
   return 0;
}

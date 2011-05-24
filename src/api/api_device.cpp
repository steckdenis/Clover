#include "CL/cl.h"
#include <core/cpudevice.h>

cl_int
clGetDeviceIDs(cl_platform_id   platform,
               cl_device_type   device_type,
               cl_uint          num_entries,
               cl_device_id *   devices,
               cl_uint *        num_devices)
{
    if (platform != 0)
        // We currently implement only one platform
        return CL_INVALID_PLATFORM;
    
    if (num_entries == 0 && devices != 0)
        return CL_INVALID_VALUE;
    
    if (num_devices == 0 && devices == 0)
        return CL_INVALID_VALUE;
    
    // We currently implement only CPU-based acceleration
    if (device_type & (CL_DEVICE_TYPE_DEFAULT | CL_DEVICE_TYPE_CPU))
    {
        if (devices)
            devices[0] = (cl_device_id)(new Coal::CPUDevice());
        
        if (num_devices)
            *num_devices = 1;
    }
    else
        return CL_DEVICE_NOT_FOUND;
    
    return CL_SUCCESS;
}

cl_int
clGetDeviceInfo(cl_device_id    device,
                cl_device_info  param_name,
                size_t          param_value_size,
                void *          param_value,
                size_t *        param_value_size_ret)
{
    if (device == 0)
        // NOTE: Check that device is really a pointer to a DeviceInterface ?
        return CL_INVALID_DEVICE;
    
    Coal::DeviceInterface *iface = (Coal::DeviceInterface *)device;
    return iface->info(param_name, param_value_size, param_value, 
                       param_value_size_ret);
}

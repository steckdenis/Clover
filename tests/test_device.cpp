#include "test_device.h"
#include "CL/cl.h"

START_TEST (test_get_device_ids)
{
    cl_platform_id platform = 0;
    cl_device_id device;
    cl_uint num_devices;
    cl_int result;
    
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, &device, &num_devices);
    fail_if(
        result != CL_INVALID_VALUE,
        "num_entries cannot be NULL when devices is not null"
    );
    
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, 0, &num_devices);
    fail_if(
        result != CL_INVALID_VALUE,
        "num_entires and devices cannot be NULL at the same time"
    );
    
    result = clGetDeviceIDs((cl_platform_id)1337, CL_DEVICE_TYPE_CPU, 1, &device, &num_devices);
    fail_if(
        result != CL_INVALID_PLATFORM,
        "1337 is not a valid platform"
    );
    
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);
    fail_if(
        result != CL_DEVICE_NOT_FOUND,
        "there are no GPU devices currently available"
    );
    
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, 0, &num_devices);
    fail_if(
        result != CL_SUCCESS || num_devices != 1,
        "we must succeed and say that we have one CPU device"
    );
    
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, &num_devices);
    fail_if(
        result != CL_SUCCESS || num_devices != 1 || device == 0,
        "we must succeed and have one CPU device"
    );
}
END_TEST

TCase *cl_device_tcase_create(void)
{
    TCase *tc = NULL;
    tc = tcase_create("device");
    tcase_add_test(tc, test_get_device_ids);
    return tc;
}

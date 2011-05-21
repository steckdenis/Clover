#include "test_context.h"
#include "CL/cl.h"

#include <stdio.h>

START_TEST (test_create_context)
{
    cl_platform_id platform = 0;
    cl_device_id device, wrong_device;
    cl_int result;
    cl_context ctx;
    
    struct __attribute__((packed)) {
        cl_context_properties prop_platform;
        cl_platform_id platform;
        cl_context_properties null;
    } _properties;
    
    const cl_context_properties *properties = 
        (const cl_context_properties *)&_properties;
    
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, 0);
    fail_if(
        result != CL_SUCCESS,
        "unable to get a device"
    );
    
    _properties.prop_platform = CL_CONTEXT_PLATFORM;
    _properties.null = 0;
    
    ctx = clCreateContext(properties, 1, 0, 0, 0, &result);
    fail_if(
        result != CL_INVALID_VALUE || ctx != 0,
        "devices cannot be NULL"
    );
    
    ctx = clCreateContext(properties, 0, &device, 0, 0, &result);
    fail_if(
        result != CL_INVALID_VALUE || ctx != 0,
        "num_devices cannot be 0"
    );
    
    _properties.platform = (cl_platform_id)1337;
    
    ctx = clCreateContext(properties, 1, &device, 0, 0, &result);
    fail_if(
        result != CL_INVALID_PLATFORM || ctx != 0,
        "1337 is not a valid platform"
    );
    
    _properties.platform = platform;
    _properties.prop_platform = 1337;
    
    ctx = clCreateContext(properties, 1, &device, 0, 0, &result);
    fail_if(
        result != CL_INVALID_PROPERTY || ctx != 0,
        "1337 is not a valid cl_context_properties"
    );
    
    _properties.prop_platform = CL_CONTEXT_PLATFORM;
    
    ctx = clCreateContext(properties, 1, &device, 0, (void *)&device, &result);
    fail_if(
        result != CL_INVALID_VALUE || ctx != 0,
        "user_data must be NULL if pfn_notify is NULL"
    );
    
    wrong_device = 0;
    
    ctx = clCreateContext(properties, 1, &wrong_device, 0, 0, &result);
    fail_if(
        result != CL_INVALID_DEVICE || ctx != 0,
        "0 is not a valid device"
    );
    
    ctx = clCreateContext(properties, 1, &device, 0, 0, &result);
    printf("rs : %i, ctx : %p\n", result, ctx);
    fail_if(
        result != CL_SUCCESS || ctx == 0,
        "unable to create a valid context"
    );
    
    clReleaseContext(ctx);
    
    ctx = clCreateContext(properties, 1, &device, 0, 0, 0);
    fail_if(
        ctx == 0,
        "errcode_ret can be NULL"
    );
}
END_TEST

TCase *cl_context_tcase_create(void)
{
    TCase *tc = NULL;
    tc = tcase_create("context");
    tcase_add_test(tc, test_create_context);
    return tc;
}

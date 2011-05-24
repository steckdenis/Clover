#include "test_context.h"
#include "CL/cl.h"

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

START_TEST (test_create_context_from_type)
{
    cl_context ctx;
    cl_int result;
    
    ctx = clCreateContextFromType(0, CL_DEVICE_TYPE_DEFAULT, 0, 0, &result);
    fail_if(
        result != CL_SUCCESS || ctx == 0,
        "unable to create a valid context with a device of type default"
    );
}
END_TEST

START_TEST (test_get_context_info)
{
    cl_context ctx;
    cl_int result;
    size_t size_ret;
    
    union {
        cl_uint refcount, num_devices;
        cl_device_id device;
        struct __attribute__((packed)) {
            cl_context_properties prop_platform;
            cl_platform_id platform;
            cl_context_properties null;
        } properties;
    } context_info;
    
    const cl_context_properties *properties = 
        (const cl_context_properties *)&context_info.properties;
    
    // Test for a dummy context
    ctx = clCreateContextFromType(0, CL_DEVICE_TYPE_DEFAULT, 0, 0, &result);
    fail_if(
        result != CL_SUCCESS || ctx == 0,
        "unable to create a valid context with a device of type default"
    );
    
    result = clGetContextInfo(0, CL_CONTEXT_REFERENCE_COUNT, 0, 0, &size_ret);
    fail_if(
        result != CL_INVALID_CONTEXT,
        "0 is not a valid context"
    );
    
    result = clGetContextInfo(ctx, 1337, 0, 0, &size_ret);
    fail_if(
        result != CL_INVALID_VALUE,
        "1337 is not a valid param_name"
    );
    
    result = clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT, 0, &context_info,
                              &size_ret);
    fail_if(
        result != CL_INVALID_VALUE,
        "param_value_size is too small to contain a cl_uint"
    );
    
    result = clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT, 0, 0, &size_ret);
    fail_if(
        result != CL_SUCCESS || size_ret != sizeof(cl_uint),
        "we must succeed and say that we'll return a cl_uint"
    );
    
    // Use a real context and check the return values
    clReleaseContext(ctx);
    
    context_info.properties.prop_platform = CL_CONTEXT_PLATFORM;
    context_info.properties.platform = 0;
    context_info.properties.null = 0;
    
    ctx = clCreateContextFromType(properties, CL_DEVICE_TYPE_DEFAULT, 0, 0,
                                  &result);
    fail_if(
        result != CL_SUCCESS || ctx == 0,
        "unable to create a valid context with a device of type default"
    );
    
    // This call clobbers context_info.properties, so we also check that
    // they are properly memcpy'ed by Coal::Context.
    result = clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint),
                              &context_info, &size_ret);
    fail_if(
        result != CL_SUCCESS || context_info.refcount != 1,
        "context's reference count must be 1 here"
    );
    
    clRetainContext(ctx);
    
    result = clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint),
                              &context_info, &size_ret);
    fail_if(
        result != CL_SUCCESS || size_ret != sizeof(cl_uint) || 
        context_info.refcount != 2,
        "context's reference count must be 2 here"
    );
    
    result = clGetContextInfo(ctx, CL_CONTEXT_NUM_DEVICES, sizeof(cl_uint),
                              &context_info, &size_ret);
    fail_if(
        result != CL_SUCCESS || size_ret != sizeof(cl_uint) || 
        context_info.num_devices != 1,
        "we currently support only one device : CPU"
    );
    
    result = clGetContextInfo(ctx, CL_CONTEXT_DEVICES, sizeof(cl_device_id),
                              &context_info, &size_ret);
    fail_if(
        result != CL_SUCCESS || size_ret != sizeof(cl_device_id) || 
        context_info.device == 0,
        "this context must have a device"
    );
    
    result = clGetContextInfo(ctx, CL_CONTEXT_PROPERTIES, 
                              sizeof(context_info.properties), &context_info,
                              &size_ret);
    fail_if(
        result != CL_SUCCESS || size_ret != sizeof(context_info.properties) || 
        context_info.properties.prop_platform != CL_CONTEXT_PLATFORM,
        "this context must have a valid CL_CONTEXT_PLATFORM property"
    );
}
END_TEST

TCase *cl_context_tcase_create(void)
{
    TCase *tc = NULL;
    tc = tcase_create("context");
    tcase_add_test(tc, test_create_context);
    tcase_add_test(tc, test_create_context_from_type);
    tcase_add_test(tc, test_get_context_info);
    return tc;
}

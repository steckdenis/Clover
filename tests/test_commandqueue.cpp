#include "test_commandqueue.h"
#include "CL/cl.h"

#include <stdio.h>

START_TEST (test_create_command_queue)
{
    cl_platform_id platform = 0;
    cl_device_id device;
    cl_context ctx;
    cl_command_queue queue;
    cl_int result;
    
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, 0);
    fail_if(
        result != CL_SUCCESS,
        "unable to get the default device"
    );
    
    ctx = clCreateContext(0, 1, &device, 0, 0, &result);
    fail_if(
        result != CL_SUCCESS || ctx == 0,
        "unable to create a valid context"
    );
    
    queue = clCreateCommandQueue(0, device, 0, &result);
    fail_if(
        result != CL_INVALID_CONTEXT,
        "context must be valid"
    );
    
    queue = clCreateCommandQueue(ctx, 0, 0, &result);
    fail_if(
        result != CL_INVALID_DEVICE,
        "device cannot be NULL"
    );
    
    // HACK : may crash if implementation changes, even if the test should pass.
    queue = clCreateCommandQueue(ctx, (cl_device_id)1337, 0, &result);
    fail_if(
        result != CL_INVALID_DEVICE,
        "1337 is not a device associated to the context"
    );
    
    queue = clCreateCommandQueue(ctx, device, 1337, &result);
    fail_if(
        result != CL_INVALID_VALUE,
        "1337 is not a valid value for properties"
    );
    
    queue = clCreateCommandQueue(ctx, device, 0, &result);
    fail_if(
        result != CL_SUCCESS || queue == 0,
        "cannot create a command queue"
    );
    
    clReleaseCommandQueue(queue);
    clReleaseContext(ctx);
}
END_TEST

START_TEST (test_get_command_queue_info)
{
    cl_platform_id platform = 0;
    cl_device_id device;
    cl_context ctx;
    cl_command_queue queue;
    cl_int result;
    
    union {
        cl_context ctx;
        cl_device_id device;
        cl_uint refcount;
        cl_command_queue_properties properties;
    } info;
    
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, 0);
    fail_if(
        result != CL_SUCCESS,
        "unable to get the default device"
    );
    
    ctx = clCreateContext(0, 1, &device, 0, 0, &result);
    fail_if(
        result != CL_SUCCESS || ctx == 0,
        "unable to create a valid context"
    );
    
    queue = clCreateCommandQueue(ctx, device, 0, &result);
    fail_if(
        result != CL_SUCCESS || queue == 0,
        "cannot create a command queue"
    );
    
    result = clGetCommandQueueInfo(queue, CL_QUEUE_CONTEXT, sizeof(cl_context),
                                   (void *)&info, 0);
    fail_if(
        result != CL_SUCCESS || info.ctx != ctx,
        "the queue doesn't retain its context"
    );
    
    result = clGetCommandQueueInfo(queue, CL_QUEUE_DEVICE, sizeof(cl_device_id),
                                   (void *)&info, 0);
    fail_if(
        result != CL_SUCCESS || info.device != device,
        "the queue doesn't retain its device"
    );
    
    result = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(cl_uint),
                                   (void *)&info, 0);
    fail_if(
        result != CL_SUCCESS || info.refcount != 1,
        "the queue must have a refcount of 1 when it's created"
    );
    
    result = clGetCommandQueueInfo(queue, CL_QUEUE_PROPERTIES, sizeof(cl_command_queue_properties),
                                   (void *)&info, 0);
    fail_if(
        result != CL_SUCCESS || info.properties != 0,
        "we gave no properties to the command queue"
    );
    
    clReleaseCommandQueue(queue);
    clReleaseContext(ctx);
}
END_TEST

START_TEST (test_object_tree)
{
    cl_platform_id platform = 0;
    cl_device_id device;
    cl_context ctx;
    cl_command_queue queue;
    cl_int result;
    cl_uint refcount;
    
    result = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, 0);
    fail_if(
        result != CL_SUCCESS,
        "unable to get the default device"
    );
    
    ctx = clCreateContext(0, 1, &device, 0, 0, &result);
    fail_if(
        result != CL_SUCCESS || ctx == 0,
        "unable to create a valid context"
    );
    
    queue = clCreateCommandQueue(ctx, device, 0, &result);
    fail_if(
        result != CL_SUCCESS || queue == 0,
        "cannot create a command queue"
    );
    
    result = clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint),
                              (void *)&refcount, 0);
    fail_if(
        result != CL_SUCCESS || refcount != 2,
        "the queue must increment the refcount of its context"
    );
    
    clReleaseCommandQueue(queue);
    
    result = clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint),
                              (void *)&refcount, 0);
    fail_if(
        result != CL_SUCCESS || refcount != 1,
        "the queue must decrement the refcount of its context when it's destroyed"
    );
    
    clReleaseContext(ctx);
}
END_TEST

TCase *cl_commandqueue_tcase_create(void)
{
    TCase *tc = NULL;
    tc = tcase_create("commandqueue");
    tcase_add_test(tc, test_create_command_queue);
    tcase_add_test(tc, test_get_command_queue_info);
    tcase_add_test(tc, test_object_tree);
    return tc;
}

#include "test_context.h"

#include <OpenCL/cl.h>

START_TEST (test_create_context)
{
    cl_context context = NULL;
    cl_device_id device;
    cl_int result = -1;
    cl_int err_code;
    cl_int result_context;

    result = clGetDeviceIDs(CL_DEVICE_TYPE_CPU, 1, &device, NULL);
    if(result == CL_SUCCESS) {
        //in clover we need plataform argument?
        context = clCreateContext(0, 1, &device, NULL, NULL, NULL);
        fail_if(context == NULL, "It should work, context not created");
        result_context = clReleaseContext(context);
        fail_if((result_context != CL_SUCCESS) || (context != NULL),
                "It should work, context no released");

        context = clCreateContext(0, 1, NULL, NULL, NULL, &err_code);
        fail_if((context != NULL) || (err_code != CL_INVALID_VALUE),
                "It should not work, context or err_code returning wrong");
        result_context = clReleaseContext(context);

        context = clCreateContext(0, 0, &device, NULL, NULL, &err_code);
        fail_if((context != NULL) || (err_code != CL_INVALID_VALUE),
                "It should not work, context or err_code returning wrong");
        result_context = clReleaseContext(context);
    }
}
END_TEST

START_TEST (test_create_context_from_type)
{
    cl_int result = -1;
    cl_context context = NULL;

    context = clCreateContextFromType(0, CL_DEVICE_TYPE_CPU, NULL, NULL, NULL);
    fail_if(context == NULL, "It should work, context not created");
}
END_TEST

TCase *cl_context_tcase_create(void)
{
    TCase *tc = NULL;
    tc = tcase_create("context");
    tcase_add_test(tc, test_create_context);
    tcase_add_test(tc, test_create_context_from_type);
    return tc;
}

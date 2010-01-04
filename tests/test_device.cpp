#include "test_device.h"

#include <OpenCL/cl.h>

#define CL_DEVICE_TYPE_INVALID 0x30 //i hope it be an invalid device

static cl_device_id device;

START_TEST (test_get_device_ids)
{
    cl_int result = -1;

    result = clGetDeviceIDs(CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
    fail_if(result != CL_SUCCESS, "It is not possible get default device id");

    result = clGetDeviceIDs(CL_DEVICE_TYPE_CPU, 1, &device, NULL);
    fail_if(result != CL_SUCCESS, "It is not possible get cpu device id");

    result = clGetDeviceIDs(CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    fail_if(result != CL_SUCCESS, "It is not possible get gpu device id");

    result = clGetDeviceIDs(CL_DEVICE_TYPE_ACCELERATOR, 1, &device, NULL);
    fail_if(result != CL_SUCCESS, "It is not possible get accelarator device id");

    result = clGetDeviceIDs(CL_DEVICE_TYPE_ALL, 1, &device, NULL);
    fail_if(result != CL_SUCCESS, "It is not possible get all device ids");


    result = clGetDeviceIDs(CL_DEVICE_TYPE_DEFAULT, 0, &device, NULL);
    fail_if(result != CL_INVALID_VALUE, "It should not be working");

    result = clGetDeviceIDs(CL_DEVICE_TYPE_DEFAULT, 1, NULL, NULL);
    fail_if(result != CL_INVALID_VALUE, "It should not be working");


    result = clGetDeviceIDs(CL_DEVICE_TYPE_INVALID, 1, &device, NULL);
    fail_if(result != CL_INVALID_DEVICE_TYPE, "It should not be working,\
            device type does not exist");
}
END_TEST

START_TEST (test_get_device_info)
{
    cl_int result = -1;

    result = clGetDeviceIDs(CL_DEVICE_TYPE_CPU, 1, &device, NULL);
    if(result == CL_SUCCESS) {
        cl_int device_info_result;
        cl_device_type device_type;

        device_info_result = clGetDeviceInfo(device, CL_DEVICE_TYPE,
                sizeof(device_type), &device_type, NULL);
        fail_if((device_info_result != CL_SUCCESS) ||
                (device_type != CL_DEVICE_TYPE_CPU), "It should work");

        //FIXME: check all cl_device_info

        device_info_result = clGetDeviceInfo(0x0, CL_DEVICE_TYPE,
                sizeof(device_type), &device_type, NULL);
        fail_if(device_info_result != CL_INVALID_DEVICE, "It should not be working,\
                invalid device");

        device_info_result = clGetDeviceInfo(device, CL_DEVICE_TYPE_INVALID,
                sizeof(device_type), &device_type, NULL);
        fail_if(device_info_result != CL_INVALID_VALUE,"It should not be working,\
                invalid param name");

        device_info_result = clGetDeviceInfo(device, CL_DEVICE_TYPE,
                sizeof(device_type), NULL, NULL);
        // in opencl specification if param_value is NULL it should be ignored
        // device_info_result should return cl_success? we think so
        fail_if(device_info_result != CL_SUCCESS, "It should work");

        device_info_result = clGetDeviceInfo(device, CL_DEVICE_TYPE,
                sizeof(device_type)-10, &device_type, NULL);
        fail_if(device_info_result != CL_INVALID_VALUE, "It should not be working,\
                param_value_size is < size of return type");
    }
}
END_TEST

TCase *cl_device_tcase_create(void)
{
    TCase *tc = NULL;
    tc = tcase_create("device");
    tcase_add_test(tc, test_get_device_ids);
    tcase_add_test(tc, test_get_device_info);
    return tc;
}

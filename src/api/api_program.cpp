#include "CL/cl.h"

// Program Object APIs
cl_program
clCreateProgramWithSource(cl_context        context,
                          cl_uint           count,
                          const char **     strings,
                          const size_t *    lengths,
                          cl_int *          errcode_ret)
{
    return 0;
}

cl_program
clCreateProgramWithBinary(cl_context            context,
                          cl_uint               num_devices,
                          const cl_device_id *  device_list,
                          const size_t *        lengths,
                          const void **         binaries,
                          cl_int *              binary_status,
                          cl_int *              errcode_ret)
{
    return 0;
}

cl_int
clRetainProgram(cl_program program)
{
    return 0;
}

cl_int
clReleaseProgram(cl_program program)
{
    return 0;
}

cl_int
clBuildProgram(cl_program           program,
               cl_uint              num_devices,
               const cl_device_id * device_list,
               const char *         options,
               void (*pfn_notify)(cl_program program, void * user_data),
               void *               user_data)
{
    return 0;
}

cl_int
clUnloadCompiler(void)
{
    return 0;
}

cl_int
clGetProgramInfo(cl_program         program,
                 cl_program_info    param_name,
                 size_t             param_value_size,
                 void *             param_value,
                 size_t *           param_value_size_ret)
{
    return 0;
}

cl_int
clGetProgramBuildInfo(cl_program            program,
                      cl_device_id          device,
                      cl_program_build_info param_name,
                      size_t                param_value_size,
                      void *                param_value,
                      size_t *              param_value_size_ret)
{
    return 0;
}

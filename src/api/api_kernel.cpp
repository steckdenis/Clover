#include "CL/cl.h"

// Kernel Object APIs
cl_kernel
clCreateKernel(cl_program      program,
               const char *    kernel_name,
               cl_int *        errcode_ret)
{
    return 0;
}

cl_int
clCreateKernelsInProgram(cl_program     program,
                         cl_uint        num_kernels,
                         cl_kernel *    kernels,
                         cl_uint *      num_kernels_ret)
{
    return 0;
}

cl_int
clRetainKernel(cl_kernel    kernel)
{
    return 0;
}

cl_int
clReleaseKernel(cl_kernel   kernel)
{
    return 0;
}

cl_int
clSetKernelArg(cl_kernel    kernel,
               cl_uint      arg_indx,
               size_t       arg_size,
               const void * arg_value)
{
    return 0;
}

cl_int
clGetKernelInfo(cl_kernel       kernel,
                cl_kernel_info  param_name,
                size_t          param_value_size,
                void *          param_value,
                size_t *        param_value_size_ret)
{
    return 0;
}

cl_int
clGetKernelWorkGroupInfo(cl_kernel                  kernel,
                         cl_device_id               device,
                         cl_kernel_work_group_info  param_name,
                         size_t                     param_value_size,
                         void *                     param_value,
                         size_t *                   param_value_size_ret)
{
    return 0;
}

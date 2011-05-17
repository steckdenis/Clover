#include <CL/cl.h>

// Context APIs

cl_context
clCreateContext(const cl_context_properties  *properties,
                cl_uint                       num_devices,
                const cl_device_id *          devices,
                void (*pfn_notify)(const char *, const void *, size_t, void *),
                void *                        user_data,
                cl_int *                      errcode_ret)
{
    return 0;
}

cl_context
clCreateContextFromType(const cl_context_properties   *properties,
                        cl_device_type          device_type,
                        void (*pfn_notify)(const char *, const void *, size_t, void *),
                        void *                  user_data,
                        cl_int *                errcode_ret)
{
    return 0;
}

cl_int
clRetainContext(cl_context context)
{
    return 0;
}

cl_int
clReleaseContext(cl_context context)
{
    return 0;
}

cl_int
clGetContextInfo(cl_context         context,
                 cl_context_info    param_name,
                 size_t             param_value_size,
                 void *             param_value,
                 size_t *           param_value_size_ret)
{
    return 0;
}

#include <CL/cl.h>
#include <core/context.h>

// Context APIs

cl_context
clCreateContext(const cl_context_properties  *properties,
                cl_uint                       num_devices,
                const cl_device_id *          devices,
                void (CL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *),
                void *                        user_data,
                cl_int *                      errcode_ret)
{
    cl_int default_errcode_ret;
    
    // No errcode_ret ? 
    if (!errcode_ret)
        errcode_ret = &default_errcode_ret;
    
    if (!devices || 
        !num_devices ||
        (!pfn_notify && user_data))
    {
        *errcode_ret = CL_INVALID_VALUE;
        return 0;
    }
    
    *errcode_ret = CL_SUCCESS;
    Coal::Context *ctx = new Coal::Context(properties, num_devices, devices,
                                           pfn_notify, user_data, errcode_ret);
    
    if (*errcode_ret != CL_SUCCESS)
    {
        // Initialization failed, destroy context
        delete ctx;
        return 0;
    }
    
    return (_cl_context *)ctx;
}

cl_context
clCreateContextFromType(const cl_context_properties   *properties,
                        cl_device_type          device_type,
                        void (CL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *),
                        void *                  user_data,
                        cl_int *                errcode_ret)
{
    return 0;
}

cl_int
clRetainContext(cl_context context)
{
    if (!context)
        return CL_INVALID_CONTEXT;
    
    context->reference();
    
    return CL_SUCCESS;
}

cl_int
clReleaseContext(cl_context context)
{
    if (!context)
        return CL_INVALID_CONTEXT;
        
    if (context->dereference())
        delete context;
    
    return CL_SUCCESS;
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

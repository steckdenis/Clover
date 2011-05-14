#include <CL/cl.h>
#include <core/context.h>

// Context APIs

cl_context
clCreateContext(const cl_context_properties  *properties,
                cl_uint                       num_devices,
                const cl_device_id *          devices,
                void (*pfn_notify)(const char *, const void *, size_t, void *),
                void *                        user_data,
                cl_int *                      errcode_ret)
{
    cl_context ret_context = NULL;
    cl_device_type type;
    cl_device_id device = devices[0];
    cl_int device_info;

    device_info = clGetDeviceInfo(device, CL_DEVICE_TYPE,
                                  sizeof(type), &type, NULL);
    if (device_info != CL_INVALID_DEVICE) {
        ret_context =  clCreateContextFromType(properties, type,
                pfn_notify, user_data, errcode_ret);
    }

    return ret_context;
}

cl_context
clCreateContextFromType(const cl_context_properties   *properties,
                        cl_device_type          device_type,
                        void (*pfn_notify)(const char *, const void *, size_t, void *),
                        void *                  user_data,
                        cl_int *                errcode_ret)
{
   struct _cl_context *context = NULL;

    switch (device_type) {
    case CL_DEVICE_TYPE_CPU:
       context = new _cl_context();

        break;
    default:
        if (errcode_ret) {
            *errcode_ret = CL_INVALID_DEVICE_TYPE;
        }
    }

    return context;
}

cl_int
clRetainContext(cl_context context)
{
    cl_int ret;

    if (context) {
        context->ref();
        ret = CL_SUCCESS;
    } else {
        ret = CL_INVALID_CONTEXT;
    }

    return ret;
}

cl_int
clReleaseContext(cl_context context)
{
    cl_uint ret;

    if (context) {
        context->deref();
        ret = CL_SUCCESS;
    } else {
        ret = CL_INVALID_CONTEXT;
    }

    return ret;
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

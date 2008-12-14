#include <OpenCL/cl.h>

// Command Queue APIs
cl_command_queue
clCreateCommandQueue(cl_context                     context,
                     cl_device_id                   device,
                     cl_command_queue_properties    properties,
                     cl_int *                       errcode_ret)
{
    return 0;
}

cl_int
clRetainCommandQueue(cl_command_queue command_queue)
{
    return 0;
}

cl_int
clReleaseCommandQueue(cl_command_queue command_queue)
{
    return 0;
}

cl_int
clGetCommandQueueInfo(cl_command_queue      command_queue,
                      cl_command_queue_info param_name,
                      size_t                param_value_size,
                      void *                param_value,
                      size_t *              param_value_size_ret)
{
    return 0;
}

cl_int
clSetCommandQueueProperty(cl_command_queue              command_queue,
                          cl_command_queue_properties   properties,
                          cl_int                        enable,
                          cl_command_queue_properties * old_properties)
{
    return 0;
}

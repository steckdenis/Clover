#include <OpenCL/cl.h>

// Event Object APIs
cl_int
clWaitForEvents(cl_uint             num_events,
                const cl_event *    event_list)
{
    return 0;
}

cl_int
clGetEventInfo(cl_event         event,
               cl_event_info    param_name,
               size_t           param_value_size,
               void *           param_value,
               size_t *         param_value_size_ret)
{
    return 0;
}

cl_int
clRetainEvent(cl_event event)
{
    return 0;
}

cl_int
clReleaseEvent(cl_event event)
{
    return 0;
}

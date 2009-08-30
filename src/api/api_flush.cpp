#include <OpenCL/cl.h>

// Flush and Finish APIs
cl_int
clFlush(cl_command_queue command_queue)
{
    return 0;
}

cl_int
clFinish(cl_command_queue command_queue)
{
    return 0;
}

#include "CL/cl.h"

// Platform API

cl_int CL_API_CALL
clGetPlatformIDs(cl_uint          num_entries,
                 cl_platform_id * platforms,
                 cl_uint *        num_platforms)
{
   return CL_SUCCESS;
}

cl_int
clGetPlatformInfo(cl_platform_info param_name,
                  size_t           param_value_size,
                  void *           param_value,
                  size_t *         param_value_size_ret)
{
   return CL_SUCCESS;
}

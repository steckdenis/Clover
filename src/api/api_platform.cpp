#include <OpenCL/cl.h>

#include <string.h>

#define PROFILE_STR "FULL_PROFILE"
#define PROFILE_STR_LEN 12

#define VERSION_STR "OpenCL 1.0"
#define VERSION_STR_LEN 10

// Platform API
cl_int
clGetPlatformInfo(cl_platform_info param_name,
                  size_t           param_value_size,
                  void *           param_value,
                  size_t *         param_value_size_ret)
{
   switch(param_name) {
   case CL_PLATFORM_PROFILE:
      strcpy((char*)param_value, PROFILE_STR);
      *param_value_size_ret = PROFILE_STR_LEN;
      break;

   case CL_PLATFORM_VERSION:
      strcpy((char*)param_value, VERSION_STR);
      *param_value_size_ret = VERSION_STR_LEN;
      break;

   default:
      return CL_INVALID_VALUE;
   }

   return CL_SUCCESS;
}

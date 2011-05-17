#include "CL/cl.h"
#include <string.h>

static const char platform_profile[] = "FULL_PROFILE";
static const char platform_version[] = "OpenCL 1.1 Clover 0.1";
static const char platform_name[] = "Default";
static const char platform_vendor[] = "Mesa";
static const char platform_extensions[] = "cl_khr_fp64  cl_khr_int64_base_atomics cl_khr_int64_extended_atomics";

// Platform API

cl_int CL_API_CALL
clGetPlatformIDs(cl_uint          num_entries,
                 cl_platform_id * platforms,
                 cl_uint *        num_platforms)
{
    *num_platforms = 1;
    
    if (num_entries == 0)
        return CL_INVALID_VALUE;
    
    if (platforms != 0)
        // Only one "default" platform, identified by NULL
        *platforms = 0;
    
    return CL_SUCCESS;
}

cl_int
clGetPlatformInfo(cl_platform_id   platform,
                  cl_platform_info param_name,
                  size_t           param_value_size,
                  void *           param_value,
                  size_t *         param_value_size_ret)
{
    const char *string = 0;
    unsigned long len = 0;
    
    if (platform != 0)
        // NULL or what is returned by clGetPlatformIDs, that's to say also NULL
        return CL_INVALID_PLATFORM;
    
    switch (param_name) {
        case CL_PLATFORM_PROFILE:
            string = platform_profile;
            len = sizeof(platform_profile);
            break;
            
        case CL_PLATFORM_VERSION:
            string = platform_version;
            len = sizeof(platform_version);
            break;
            
        case CL_PLATFORM_NAME:
            string = platform_name;
            len = sizeof(platform_name);
            break;
            
        case CL_PLATFORM_VENDOR:
            string = platform_vendor;
            len = sizeof(platform_vendor);
            break;
            
        case CL_PLATFORM_EXTENSIONS:
            string = platform_extensions;
            len = sizeof(platform_extensions);
            break;
    }
    
    if (string == 0)
        return CL_INVALID_VALUE;
    
    if (param_value_size < len && param_value != 0)
        return CL_INVALID_VALUE;
    
    if (param_value != 0)
        memcpy(param_value, string, len);
    
    *param_value_size_ret = len;
    
    return CL_SUCCESS;
}

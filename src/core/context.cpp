#include "context.h"
#include "deviceinterface.h"

#include <string.h>
#include <stdlib.h>

using namespace Coal;

static void default_pfn_notify(const char *, const void *, size_t, void *)
{
    return;
}

Context::Context(const cl_context_properties *properties,
                 cl_uint num_devices,
                 const cl_device_id *devices,
                 void (CL_CALLBACK *pfn_notify)(const char *, const void *,
                                                size_t, void *),
                 void *user_data,
                 cl_int *errcode_ret) 
: p_references(1), p_properties(0), p_pfn_notify(pfn_notify),
  p_user_data(user_data), p_platform(0), p_devices(0)
{
    if (!p_pfn_notify)
        p_pfn_notify = &default_pfn_notify;
    
    // Explore the properties
    if (properties)
    {
        const unsigned char *props = (const unsigned char *)properties;
        cl_context_properties prop;
        size_t props_len = 0;
        
#define GET_PROP(type, var)     \
    var = *(const type *)props;       \
    props += sizeof(type);      \
    props_len += sizeof(type);
        
        while (true)
        {
            GET_PROP(cl_context_properties, prop)
            
            if (!prop)
                break;
            
            switch (prop)
            {
                case CL_CONTEXT_PLATFORM:
                    GET_PROP(cl_platform_id, p_platform);
                    break;
                    
                default:
                    *errcode_ret = CL_INVALID_PROPERTY;
                    return;
            }
        }
        
        // properties may be allocated on the stack of the client application
        // copy it into a real buffer
        p_properties = (cl_context_properties *)malloc(props_len);
        
        if (!p_properties)
        {
            *errcode_ret = CL_OUT_OF_HOST_MEMORY;
            return;
        }
        
        memcpy((void *)p_properties, (const void *)properties, props_len);
    }
    
    // Verify that the platform is good
    if (p_platform != 0)
    {
        *errcode_ret = CL_INVALID_PLATFORM;
        return;
    }
    
    // Explore the devices
    p_devices = (DeviceInterface **)malloc(num_devices * sizeof(DeviceInterface *));
    
    if (!p_devices)
    {
        *errcode_ret = CL_OUT_OF_HOST_MEMORY;
        return;
    }
    
    for (int i = 0; i < num_devices; ++i)
    {
        cl_device_id device = devices[i];
        
        if (device == 0)
        {
            *errcode_ret = CL_INVALID_DEVICE;
            return;
        }
        
        // Verify that the device is available
        cl_bool device_available;
        
        *errcode_ret = device->info(CL_DEVICE_AVAILABLE, 
                                    sizeof(device_available),
                                    &device_available,
                                    0);
        
        if (errcode_ret != CL_SUCCESS)
            return;
        
        if (!device_available)
        {
            *errcode_ret = CL_DEVICE_NOT_AVAILABLE;
            return;
        }
        
        // Add the device to the list
        p_devices[i] = (DeviceInterface *)device;
    }
}

Context::~Context()
{
    if (p_properties)
        free((void *)p_properties);
    
    free((void *)p_devices);
}

void Context::reference()
{
    p_references++;
}

bool Context::dereference()
{
    p_references--;
    return (p_references == 0);
}
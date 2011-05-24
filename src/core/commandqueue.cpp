#include "commandqueue.h"
#include "context.h"
#include "deviceinterface.h"
#include "propertylist.h"

#include <string.h>

using namespace Coal;

CommandQueue::CommandQueue(Context *ctx,
                           DeviceInterface *device,
                           cl_command_queue_properties properties, 
                           cl_int *errcode_ret)
: p_ctx(ctx), p_device(device), p_properties(properties), p_references(1)
{
    // Increment the reference count of the context
    // We begin by doing this to be able to unconditionnally release the context
    // in the destructor, being certain that the context was actually retained.
    clRetainContext((cl_context)ctx);
    
    // Check that the device belongs to the context
    if (!ctx->hasDevice(device))
    {
        *errcode_ret = CL_INVALID_DEVICE;
        return;
    }
    
    *errcode_ret = checkProperties();
}

CommandQueue::~CommandQueue()
{
    // Release the parent context
    clReleaseContext((cl_context)p_ctx);
}

void CommandQueue::reference()
{
    p_references++;
}

bool CommandQueue::dereference()
{
    p_references--;
    return (p_references == 0);
}

cl_int CommandQueue::info(cl_context_info param_name,
                          size_t param_value_size,
                          void *param_value,
                          size_t *param_value_size_ret)
{
    void *value = 0;
    int value_length = 0;
    
    union {
        cl_uint cl_uint_var;
        cl_device_id cl_device_id_var;
        cl_context cl_context_var;
        cl_command_queue_properties cl_command_queue_properties_var;
    };
    
    switch (param_name)
    {
        case CL_QUEUE_CONTEXT:
            SIMPLE_ASSIGN(cl_context, p_ctx);
            break;
            
        case CL_QUEUE_DEVICE:
            SIMPLE_ASSIGN(cl_device_id, p_device);
            break;
            
        case CL_QUEUE_REFERENCE_COUNT:
            SIMPLE_ASSIGN(cl_uint, p_references);
            break;
            
        case CL_QUEUE_PROPERTIES:
            SIMPLE_ASSIGN(cl_command_queue_properties, p_properties);
            break;
            
        default:
            return CL_INVALID_VALUE;
    }
    
    if (param_value && param_value_size < value_length)
        return CL_INVALID_VALUE;
    
    if (param_value_size_ret)
        *param_value_size_ret = value_length;
        
    if (param_value)
        memcpy(param_value, value, value_length);
    
    return CL_SUCCESS;
}
        
cl_int CommandQueue::setProperty(cl_command_queue_properties properties,
                                 cl_bool enable,
                                 cl_command_queue_properties *old_properties)
{
    if (old_properties)
        *old_properties = p_properties;
    
    if (enable)
        p_properties |= properties;
    else
        p_properties &= ~properties;
    
    return checkProperties();
}

cl_int CommandQueue::checkProperties() const
{
    // Check that all the properties are valid
    cl_command_queue_properties properties = 
        CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE |
        CL_QUEUE_PROFILING_ENABLE;
        
    if ((p_properties & properties) != p_properties)
        return CL_INVALID_VALUE;
    
    // Check that the device handles these properties
    cl_int result;
    
    result = p_device->info(CL_DEVICE_QUEUE_PROPERTIES,
                            sizeof(cl_command_queue_properties),
                            &properties,
                            0);
    
    if (result != CL_SUCCESS)
        return result;
    
    if ((p_properties & properties) != p_properties)
        return CL_INVALID_QUEUE_PROPERTIES;
    
    return CL_SUCCESS;
}
#include <core/commandqueue.h>

#include <CL/cl.h>

// Command Queue APIs
cl_command_queue
clCreateCommandQueue(cl_context                     context,
                     cl_device_id                   device,
                     cl_command_queue_properties    properties,
                     cl_int *                       errcode_ret)
{
    cl_int default_errcode_ret;
    
    // No errcode_ret ? 
    if (!errcode_ret)
        errcode_ret = &default_errcode_ret;
    
    if (!device)
    {
        *errcode_ret = CL_INVALID_DEVICE;
        return 0;
    }
    
    if (!context)
    {
        *errcode_ret = CL_INVALID_CONTEXT;
        return 0;
    }
    
    *errcode_ret = CL_SUCCESS;
    Coal::CommandQueue *queue = new Coal::CommandQueue(
                                        (Coal::Context *)context,
                                        (Coal::DeviceInterface *)device,
                                        properties,
                                        errcode_ret);
    
    if (*errcode_ret != CL_SUCCESS)
    {
        // Initialization failed, destroy context
        delete queue;
        return 0;
    }
    
    return (_cl_command_queue *)queue;
}

cl_int
clRetainCommandQueue(cl_command_queue command_queue)
{
    if (!command_queue)
        return CL_INVALID_COMMAND_QUEUE;
    
    command_queue->reference();
    
    return CL_SUCCESS;
}

cl_int
clReleaseCommandQueue(cl_command_queue command_queue)
{
    if (!command_queue)
        return CL_INVALID_COMMAND_QUEUE;
        
    if (command_queue->dereference())
        delete command_queue;
    
    return CL_SUCCESS;
}

cl_int
clGetCommandQueueInfo(cl_command_queue      command_queue,
                      cl_command_queue_info param_name,
                      size_t                param_value_size,
                      void *                param_value,
                      size_t *              param_value_size_ret)
{
    if (!command_queue)
        return CL_INVALID_COMMAND_QUEUE;
    
    return command_queue->info(param_name, param_value_size, param_value,
                               param_value_size_ret);
}

cl_int
clSetCommandQueueProperty(cl_command_queue              command_queue,
                          cl_command_queue_properties   properties,
                          cl_bool                       enable,
                          cl_command_queue_properties * old_properties)
{
    if (!command_queue)
        return CL_INVALID_COMMAND_QUEUE;
    
    return command_queue->setProperty(properties, enable, old_properties);
}

#ifndef __COMMANDQUEUE_H__
#define __COMMANDQUEUE_H__

#include <CL/cl.h>

namespace Coal
{
    
class Context;
class DeviceInterface;

class CommandQueue
{
    public:
        CommandQueue(Context *ctx,
                     DeviceInterface *device,
                     cl_command_queue_properties properties, 
                     cl_int *errcode_ret);
        ~CommandQueue();
        
        void reference();
        bool dereference();     /*!< @return true if reference becomes 0 */
        
        cl_int info(cl_context_info param_name,
                    size_t param_value_size,
                    void *param_value,
                    size_t *param_value_size_ret);
        
        cl_int setProperty(cl_command_queue_properties properties,
                           cl_bool enable,
                           cl_command_queue_properties *old_properties);
        
        cl_int checkProperties() const;
        
    private:
        Context *p_ctx;
        DeviceInterface *p_device;
        cl_command_queue_properties p_properties;
        
        unsigned int p_references;
};

}

struct _cl_command_queue : public Coal::CommandQueue
{};

#endif
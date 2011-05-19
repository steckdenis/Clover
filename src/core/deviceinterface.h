#ifndef __DEVICEINTERFACE_H__
#define __DEVICEINTERFACE_H__

#include <CL/cl.h>

namespace Coal
{

class DeviceInterface
{
    public:
        DeviceInterface() {}
        ~DeviceInterface() {}
        
        virtual cl_int info(cl_device_info param_name, 
                            size_t param_value_size, 
                            void *param_value, 
                            size_t *param_value_size_ret) = 0;
};

}

struct _cl_device_id : public Coal::DeviceInterface
{};

#endif
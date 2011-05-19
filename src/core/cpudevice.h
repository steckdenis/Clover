#ifndef __CPUDEVICE_H__
#define __CPUDEVICE_H__

#include "deviceinterface.h"

namespace Coal
{

class CPUDevice : public DeviceInterface
{
    public:
        CPUDevice();
        ~CPUDevice();
        
        cl_int info(cl_device_info param_name, 
                    size_t param_value_size, 
                    void *param_value, 
                    size_t *param_value_size_ret);
};

}

#endif
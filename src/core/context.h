#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <CL/cl.h>

namespace Coal
{
    
class DeviceInterface;

class Context
{
    public:
        Context(const cl_context_properties *properties,
                cl_uint num_devices,
                const cl_device_id *devices,
                void (CL_CALLBACK *pfn_notify)(const char *, const void *,
                                               size_t, void *),
                void *user_data,
                cl_int *errcode_ret);
        ~Context();
        
        void reference();
        bool dereference();     /*!< @return true if reference becomes 0 */ 
        
    private:
        cl_context_properties *p_properties;
        void (CL_CALLBACK *p_pfn_notify)(const char *, const void *,
                                               size_t, void *);
        void *p_user_data;
        
        DeviceInterface **p_devices;
        unsigned int p_references;
        cl_platform_id p_platform;
};

}

struct _cl_context : public Coal::Context
{};

#endif
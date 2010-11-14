#include "CL/cl.h"
#include "CL/cl_platform.h"

#include "core/device.h"

#include "pipe/p_screen.h"
#include "pipe/p_format.h"
#include "util/u_memory.h"

#include <string>

// Device APIs

struct CLConstValue {
   CLConstValue(int _id)
      : id(_id)
   {}

   virtual void param(size_t param_value_size,
                      void * param_value,
                      size_t *param_value_size_ret)
   {}

   int id;
};
template <class  T>
struct CLConstValueTemplate : public CLConstValue {
   CLConstValueTemplate(int _id, T _value)
      : CLConstValue(id), value(_value)
   {}
   T value;
};

const CLConstValue values[] = {
   CLConstValueTemplate<std::string>(CL_DEVICE_NAME, std::string("hello")),
   CLConstValueTemplate<int>(CL_DEVICE_TYPE, (int)3)
};

static void
create_gpu_device(cl_device_id *   devices,
                  cl_uint *        num_devices,
                  cl_uint          num_entries)
{
}

static void
create_cpu_device(cl_device_id *   devices,
                  cl_uint *        num_devices,
                  cl_uint          num_entries)
{
    Coal::DeviceId *device = Coal::DeviceId::create(CL_DEVICE_TYPE_CPU);

    devices[0] = (cl_device_id)device;
    *num_devices = 1;
}

static void
create_accel_device(cl_device_id *   devices,
                    cl_uint *        num_devices,
                    cl_uint          num_entries)
{
}

cl_int
clGetDeviceIDs(cl_platform_id   platform,
               cl_device_type   device_type,
               cl_uint          num_entries,
               cl_device_id *   devices,
               cl_uint *        num_devices)
{
    cl_bool gpu, cpu, accelerator;
    cl_uint original_num_entries = num_entries;

    gpu = (device_type & CL_DEVICE_TYPE_DEFAULT) ||
          (device_type & CL_DEVICE_TYPE_GPU) ||
          !(device_type ^ CL_DEVICE_TYPE_ALL);

    cpu = (device_type & CL_DEVICE_TYPE_CPU) ||
          !(device_type ^ CL_DEVICE_TYPE_ALL);

    accelerator = (device_type & CL_DEVICE_TYPE_ACCELERATOR) ||
                  !(device_type ^ CL_DEVICE_TYPE_ALL);

    if (!gpu && !cpu && !accelerator)
        return CL_INVALID_DEVICE_TYPE;

    if ((!num_entries && devices) || (!num_devices && !devices))
        return CL_INVALID_VALUE;

    if (gpu && num_entries > 0) {
        cl_uint num_gpus = 0;
        create_gpu_device(devices, &num_gpus, num_entries);
        num_entries -= num_gpus;
        if (num_devices)
           *num_devices += num_gpus;
    }

    if (cpu && num_entries > 0) {
        cl_uint num_cpus = 0;
        create_cpu_device(devices, &num_cpus, num_entries);
        num_entries -= num_cpus;
        if (num_devices)
           *num_devices += num_cpus;
    }

    if (accelerator && num_entries) {
        cl_uint num_accels = 0;
        create_accel_device(devices, &num_accels, num_entries);
        num_entries -= num_accels;
        if (num_devices)
           *num_devices += num_accels;
    }

    if (original_num_entries == num_entries)
        return CL_DEVICE_NOT_FOUND;

    return CL_SUCCESS;
}

cl_int
clGetDeviceInfo(cl_device_id    device,
                cl_device_info  opcode,
                size_t          param_value_size,
                void *          param_value,
                size_t *        param_value_size_ret)
{
   if (!device)
      return CL_INVALID_DEVICE;

   return device->info(opcode, param_value_size, param_value,
                       param_value_size_ret);
}

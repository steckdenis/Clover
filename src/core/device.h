#ifndef DEVICE_H
#define DEVICE_H

#include "deviceinfo.h"

#include "OpenCL/cl.h"

struct pipe_screen;


class Device
{
public:
   static Device *create(cl_uint type);
public:
   inline cl_uint type() const;
   inline struct pipe_screen *screen() const;

   cl_int info(cl_device_info  opcode,
               size_t          paramValueSize,
               void *          paramValue,
               size_t *        paramValueSizeRet) const;

private:
   Device(cl_uint type, struct pipe_screen *screen);
   void fillInfo(cl_uint type);

private:
   DeviceInfo m_info;

   struct pipe_screen *m_screen;
};

inline cl_uint Device::type() const
{
   return m_info.type;
}

inline struct pipe_screen *Device::screen() const
{
   return m_screen;
}

struct _cl_device_id : public Device
{};

#endif

#ifndef DEVICE_H
#define DEVICE_H

#include "deviceinfo.h"

#include "OpenCL/cl.h"

struct pipe_screen;
struct pipe_winsys;


class Device
{
public:
   static Device *create(cl_uint type);
public:
   inline cl_uint type() const;
   inline struct pipe_screen *screen() const;
   inline struct pipe_winsys *winsys() const;

   cl_int info(cl_device_info  opcode,
               size_t          paramValueSize,
               void *          paramValue,
               size_t *        paramValueSizeRet) const;

private:
   Device(cl_uint type, struct pipe_winsys *ws,
          struct pipe_screen *screen);
   void fillInfo(cl_uint type);

private:
   DeviceInfo m_info;

   struct pipe_winsys *m_winsys;
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

inline struct pipe_winsys *Device::winsys() const
{
   return m_winsys;
}


struct _cl_device_id : public Device
{};

#endif

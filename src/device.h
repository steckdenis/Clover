#ifndef DEVICE_H
#define DEVICE_H

#include "OpenCL/cl.h"

struct pipe_screen;
struct pipe_winsys;


class Device
{
public:
   Device(cl_uint type);

   inline cl_uint type() const;
   inline struct pipe_screen *screen() const;
   inline struct pipe_winsys *winsys() const;

private:
   struct pipe_screen *m_screen;
   struct pipe_winsys *m_winsys;

   cl_uint m_type;
};

inline cl_uint Device::type() const
{
   return m_type;
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

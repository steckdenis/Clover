#ifndef COAL_DEVICEID_H
#define COAL_DEVICEID_H

#include "deviceinfo.h"

#include "CL/cl.h"

struct pipe_screen;

namespace Coal {

   class DeviceId
   {
   public:
      static DeviceId *create(cl_uint type);
   public:
      inline cl_uint type() const;
      inline struct pipe_screen *screen() const;

      cl_int info(cl_device_info  opcode,
                  size_t          paramValueSize,
                  void *          paramValue,
                  size_t *        paramValueSizeRet) const;

   private:
      DeviceId(cl_uint type, struct pipe_screen *screen);
      void fillInfo(cl_uint type);

   private:
      DeviceInfo m_info;

      struct pipe_screen *m_screen;
   };

   inline cl_uint DeviceId::type() const
   {
      return m_info.type;
   }

   inline struct pipe_screen *DeviceId::screen() const
   {
      return m_screen;
   }
}

struct _cl_device_id : public Coal::DeviceId
{};

#endif

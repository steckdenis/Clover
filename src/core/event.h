#ifndef COAL_EVENT_H
#define COAL_EVENT_H

#include "CL/cl.h"

namespace Coal
{
   class Event
   {
   public:
      Event();
      ~Event();
   };
}

struct _cl_event : public Coal::Event
{
};

#endif

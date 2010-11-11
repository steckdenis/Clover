#ifndef COAL_PLATFORMID_H
#define COAL_PLATFORMID_H

#include "CL/cl.h"

namespace Coal {

   class PlatformId
   {
   public:
      PlatformId();
      ~PlatformId();
   };
}

struct _cl_platform_id : public Coal::PlatformId
{
};


#endif

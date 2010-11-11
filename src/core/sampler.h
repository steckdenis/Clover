#ifndef COAL_SAMPLER_H
#define COAL_SAMPLER_H

#include "CL/cl.h"

namespace Coal
{
   class Sampler
   {
   public:
      Sampler();
      ~Sampler();
   };
}

struct _cl_sampler : public Coal::Sampler
{
};

#endif

#ifndef COAL_MEM_H
#define COAL_MEM_H

#include "CL/cl.h"

namespace Coal {

   class Mem
   {
   public:
      Mem();
      ~Mem();
   };
}

struct _cl_mem : public Coal::Mem
{
};


#endif

#ifndef COAL_KERNEL_H
#define COAL_KERNEL_H

namespace Coal
{
   class Kernel
   {
   public:
      Kernel();
      ~Kernel();
   };
}

struct _cl_kernel : public Coal::Kernel
{
};

#endif

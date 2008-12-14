#ifndef DEVICE_H
#define DEVICE_H

#include "OpenCL/cl.h"

struct pipe_screen;
struct pipe_winsys;

struct _cl_device_id {
   struct pipe_screen *screen;
   struct pipe_winsys *winsys;

   cl_uint type;
};

#endif

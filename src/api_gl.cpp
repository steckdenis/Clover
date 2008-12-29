#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GL/glext.h"

#include "OpenCL/cl.h"
#include "OpenCL/cl_gl.h"

cl_mem
clCreateFromGLBuffer(cl_context     context,
                     cl_mem_flags   flags,
                     GLuint         bufobj,
                     int *          errcode_ret)
{
   return 0;
}

cl_mem
clCreateFromGLTexture2D(cl_context      context,
                        cl_mem_flags    flags,
                        GLenum          target,
                        GLint           miplevel,
                        GLuint          texture,
                        int *           errcode_ret)
{
   return 0;
}

cl_mem
clCreateFromGLTexture3D(cl_context      context,
                        cl_mem_flags    flags,
                        GLenum          target,
                        GLint           miplevel,
                        GLuint          texture,
                        int *           errcode_ret)
{
   return 0;
}

cl_mem
clCreateFromGLRenderbuffer(cl_context   context,
                           cl_mem_flags flags,
                           GLuint       renderbuffer,
                           int *        errcode_ret)
{
   return 0;
}

cl_int
clGetGLObjectInfo(cl_mem                memobj,
                  cl_gl_object_type *   gl_object_type,
                  GLuint *              gl_object_name)
{
   return 0;
}

cl_int
clGetGLTextureInfo(cl_mem               memobj,
                   cl_gl_texture_info   param_name,
                   size_t               param_value_size,
                   void *               param_value,
                   size_t *             param_value_size_ret)
{
   return 0;
}

cl_int
clEnqueueAcquireGLObjects(cl_command_queue      command_queue,
                          cl_uint               num_objects,
                          const cl_mem *        mem_objects,
                          cl_uint               num_events_in_wait_list,
                          const cl_event *      event_wait_list,
                          cl_event *            event)
{
   return 0;
}

cl_int
clEnqueueReleaseGLObjects(cl_command_queue      command_queue,
                          cl_uint               num_objects,
                          const cl_mem *        mem_objects,
                          cl_uint               num_events_in_wait_list,
                          const cl_event *      event_wait_list,
                          cl_event *            event)
{
   return 0;
}

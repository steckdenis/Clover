#include "CL/cl.h"
#include <core/memobject.h>
#include <core/context.h>


// Memory Object APIs
cl_mem
clCreateBuffer(cl_context   context,
               cl_mem_flags flags,
               size_t       size,
               void *       host_ptr,
               cl_int *     errcode_ret)
{
    cl_int dummy_errcode;
    
    if (!errcode_ret)
        errcode_ret = &dummy_errcode;
    
    if (!context)
    {
        *errcode_ret = CL_INVALID_CONTEXT;
        return 0;
    }
    
    *errcode_ret = CL_SUCCESS;
    
    Coal::Buffer *buf = new Coal::Buffer(context, size, host_ptr, flags, 
                                         errcode_ret);
    
    if (*errcode_ret != CL_SUCCESS || (*errcode_ret = buf->init()) != CL_SUCCESS)
    {
        delete buf;
        return 0;
    }
    
    return (cl_mem)buf;
}

cl_mem
clCreateSubBuffer(cl_mem                buffer,
                  cl_mem_flags          flags,
                  cl_buffer_create_type buffer_create_type,
                  const void *          buffer_create_info,
                  cl_int *              errcode_ret)
{
    cl_int dummy_errcode;
    
    if (!errcode_ret)
        errcode_ret = &dummy_errcode;
    
    if (!buffer)
    {
        *errcode_ret = CL_INVALID_MEM_OBJECT;
        return 0;
    }
    
    Coal::MemObject *memobject = (Coal::MemObject *)buffer;
    cl_buffer_region *region = (cl_buffer_region *)buffer_create_info;
    
    // NOTE: Is it right ? Couldn't we create SubBuffers of images ?
    if (memobject->type() != Coal::MemObject::Buffer)
    {
        *errcode_ret = CL_INVALID_MEM_OBJECT;
        return 0;
    }
    
    if (buffer_create_type != CL_BUFFER_CREATE_TYPE_REGION)
    {
        *errcode_ret = CL_INVALID_VALUE;
        return 0;
    }
    
    if (!buffer_create_info)
    {
        *errcode_ret = CL_INVALID_VALUE;
        return 0;
    }
    
    *errcode_ret = CL_SUCCESS;
    
    Coal::SubBuffer *buf = new Coal::SubBuffer((Coal::Buffer *)buffer,
                                               region->origin, region->size, 
                                               flags, errcode_ret);
    
    if (*errcode_ret != CL_SUCCESS || (*errcode_ret = buf->init()) != CL_SUCCESS)
    {
        delete buf;
        return 0;
    }
    
    return (cl_mem)buf;
}

cl_mem
clCreateImage2D(cl_context              context,
                cl_mem_flags            flags,
                const cl_image_format * image_format,
                size_t                  image_width,
                size_t                  image_height,
                size_t                  image_row_pitch,
                void *                  host_ptr,
                cl_int *                errcode_ret)
{
    return 0;
}

cl_mem
clCreateImage3D(cl_context              context,
                cl_mem_flags            flags,
                const cl_image_format * image_format,
                size_t                  image_width,
                size_t                  image_height,
                size_t                  image_depth,
                size_t                  image_row_pitch,
                size_t                  image_slice_pitch,
                void *                  host_ptr,
                cl_int *                errcode_ret)
{
    return 0;
}

cl_int
clRetainMemObject(cl_mem memobj)
{
    if (!memobj)
        return CL_INVALID_MEM_OBJECT;
    
    memobj->reference();
    
    return CL_SUCCESS;
}

cl_int
clReleaseMemObject(cl_mem memobj)
{
    if (!memobj)
        return CL_INVALID_MEM_OBJECT;
    
    if (memobj->dereference())
        delete memobj;
    
    return CL_SUCCESS;
}

cl_int
clGetSupportedImageFormats(cl_context           context,
                           cl_mem_flags         flags,
                           cl_mem_object_type   image_type,
                           cl_uint              num_entries,
                           cl_image_format *    image_formats,
                           cl_uint *            num_image_formats)
{
    return 0;
}

cl_int
clGetMemObjectInfo(cl_mem           memobj,
                   cl_mem_info      param_name,
                   size_t           param_value_size,
                   void *           param_value,
                   size_t *         param_value_size_ret)
{
    return 0;
}

cl_int
clGetImageInfo(cl_mem           image,
               cl_image_info    param_name,
               size_t           param_value_size,
               void *           param_value,
               size_t *         param_value_size_ret)
{
    return 0;
}

cl_int
clSetMemObjectDestructorCallback(cl_mem memobj,
                                 void   (CL_CALLBACK *pfn_notify)(cl_mem memobj,
                                                                  void *user_data),
                                 void * user_data)
{
    if (!memobj)
        return CL_INVALID_MEM_OBJECT;
    
    memobj->setDestructorCallback(pfn_notify, user_data);
    
    return CL_SUCCESS;
}


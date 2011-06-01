#include "memobject.h"
#include "context.h"
#include "deviceinterface.h"

#include <stdlib.h>
#include <stdio.h>

using namespace Coal;

/*
 * MemObject
 */

MemObject::MemObject(Context *ctx, Type type, cl_mem_flags flags, 
                     void *host_ptr, cl_int *errcode_ret)
: p_type(type), p_ctx(ctx), p_flags(flags), p_host_ptr(host_ptr), 
  p_references(1), p_dtor_callback(0), p_devicebuffers(0), p_num_devices(0)
{
    clRetainContext((cl_context)ctx);
    
    // Check the flags value
    const cl_mem_flags all_flags = CL_MEM_READ_WRITE | CL_MEM_WRITE_ONLY |
                                   CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR |
                                   CL_MEM_ALLOC_HOST_PTR | CL_MEM_COPY_HOST_PTR;

    if ((flags & ~all_flags) != 0)
    {
        *errcode_ret = CL_INVALID_VALUE;
        return;
    }
    
    if ((flags & CL_MEM_ALLOC_HOST_PTR) && (flags & CL_MEM_USE_HOST_PTR))
    {
        *errcode_ret = CL_INVALID_VALUE;
        return;
    }
    
    if ((flags & CL_MEM_COPY_HOST_PTR) && (flags & CL_MEM_USE_HOST_PTR))
    {
        *errcode_ret = CL_INVALID_VALUE;
        return;
    }
    
    // Check other values
    if ((flags & (CL_MEM_USE_HOST_PTR | CL_MEM_COPY_HOST_PTR)) != 0 && !host_ptr)
    {
        *errcode_ret = CL_INVALID_HOST_PTR;
        return;
    }
    
    if ((flags & (CL_MEM_USE_HOST_PTR | CL_MEM_COPY_HOST_PTR)) == 0 && host_ptr)
    {
        *errcode_ret = CL_INVALID_HOST_PTR;
        return;
    }
}

MemObject::~MemObject()
{
    clReleaseContext((cl_context)p_ctx);
    if (p_dtor_callback)
        p_dtor_callback((cl_mem)this, p_dtor_userdata);
    
    if (p_devicebuffers)
    {
        // Also delete our children in the device
        for (int i=0; i<p_num_devices; ++i)
            delete p_devicebuffers[i];
        
        free((void *)p_devicebuffers);
    }
}

cl_int MemObject::init()
{
    // Get the device list of the context
    DeviceInterface **devices = 0;
    cl_int rs;
    
    rs = context()->info(CL_CONTEXT_NUM_DEVICES, sizeof(uint), &p_num_devices, 0);
    
    if (rs != CL_SUCCESS)
        return rs;
    
    devices = (DeviceInterface **)malloc(p_num_devices * 
                                        sizeof(DeviceInterface *));
    
    if (!devices)
        return CL_OUT_OF_HOST_MEMORY;
    
    rs = context()->info(CL_CONTEXT_DEVICES, 
                         p_num_devices * sizeof(DeviceInterface *), devices, 0);
    
    if (rs != CL_SUCCESS)
        return rs;
    
    // Allocate a table of DeviceBuffers
    p_devicebuffers = (DeviceBuffer **)malloc(p_num_devices * 
                                             sizeof(DeviceBuffer *));
    
    if (!p_devicebuffers)
        return CL_OUT_OF_HOST_MEMORY;
    
    // Create a DeviceBuffer for each device
    for (int i=0; i<p_num_devices; ++i)
    {
        DeviceInterface *device = devices[i];
        
        p_devicebuffers[i] = device->createDeviceBuffer(this, &rs);
        
        if (rs != CL_SUCCESS)
        {
            free((void *)devices);
            return rs;
        }
    }
    
    // If we have only one device, already allocate the buffer
    if (p_num_devices == 1)
    {
        rs = p_devicebuffers[0]->allocate();
        
        if (rs != CL_SUCCESS)
            return rs;
    }
    
    return CL_SUCCESS;
}

void MemObject::reference()
{
    p_references++;
}

bool MemObject:: dereference()
{
    p_references--;
    
    return (p_references == 0);
}

MemObject::Type MemObject::type() const
{
    return p_type;
}

Context *MemObject::context() const
{
    return p_ctx;
}

cl_mem_flags MemObject::flags() const
{
    return p_flags;
}

void *MemObject::host_ptr() const
{
    return p_host_ptr;
}

DeviceBuffer *MemObject::deviceBuffer(DeviceInterface *device) const
{
    for (int i=0; i<p_num_devices; ++i)
    {
        if (p_devicebuffers[i]->device() == device)
            return p_devicebuffers[i];
    }
    
    return 0;
}

void MemObject::setDestructorCallback(void (CL_CALLBACK *pfn_notify)
                                               (cl_mem memobj, void *user_data),
                                      void *user_data)
{
    p_dtor_callback = pfn_notify;
    p_dtor_userdata = user_data;
}

/*
 * Buffer
 */

Buffer::Buffer(Context *ctx, size_t size, void *host_ptr, cl_mem_flags flags, 
               cl_int *errcode_ret)
: MemObject(ctx, MemObject::Buffer, flags, host_ptr, errcode_ret), p_size(size)
{
    if (size == 0)
    {
        *errcode_ret = CL_INVALID_BUFFER_SIZE;
        return;
    }
}

size_t Buffer::size() const
{
    return p_size;
}

/*
 * SubBuffer
 */

SubBuffer::SubBuffer(class Buffer *parent, size_t offset, size_t size, 
                     cl_mem_flags flags, cl_int *errcode_ret)
: MemObject(parent->context(), MemObject::SubBuffer, flags, 0, errcode_ret), p_size(size),
  p_offset(offset), p_parent(parent)
{
    if (size == 0)
    {
        *errcode_ret = CL_INVALID_BUFFER_SIZE;
        return;
    }
    
    if (offset + size > parent->size())
    {
        *errcode_ret = CL_INVALID_BUFFER_SIZE;
        return;
    }
    
    // Check the compatibility of flags and parent->flags()
    const cl_mem_flags wrong_flags = 
        CL_MEM_ALLOC_HOST_PTR |
        CL_MEM_USE_HOST_PTR |
        CL_MEM_COPY_HOST_PTR;
        
    if (flags & wrong_flags)
    {
        *errcode_ret = CL_INVALID_VALUE;
        return;
    }
    
    if ((parent->flags() & CL_MEM_WRITE_ONLY) &&
        (flags & (CL_MEM_READ_WRITE | CL_MEM_READ_ONLY)))
    {
        *errcode_ret = CL_INVALID_VALUE;
        return;
    }
    
    if ((parent->flags() & CL_MEM_READ_ONLY) &&
        (flags & (CL_MEM_READ_WRITE | CL_MEM_WRITE_ONLY)))
    {
        *errcode_ret = CL_INVALID_VALUE;
        return;
    }
}

size_t SubBuffer::size() const
{
    return p_size;
}

size_t SubBuffer::offset() const
{
    return p_offset;
}

Buffer *SubBuffer::parent() const
{
    return p_parent;
}

/*
 * Image2D
 */

Image2D::Image2D(Context *ctx, size_t width, size_t height, size_t row_pitch, 
                 const cl_image_format *format, void *host_ptr, 
                 cl_mem_flags flags, cl_int *errcode_ret)
: MemObject(ctx, MemObject::Image2D, flags, host_ptr, errcode_ret), 
  p_width(width), p_height(height), p_row_pitch(row_pitch), p_format(*format)
{
    // NOTE for images : pitches must be NULL if host_ptr is NULL
}

size_t Image2D::width() const
{
    return p_width;
}

size_t Image2D::height() const
{
    return p_height;
}

size_t Image2D::row_pitch() const
{
    return p_row_pitch;
}

cl_image_format Image2D::format() const
{
    return p_format;
}

/*
 * Image3D
 */

Image3D::Image3D(Context *ctx, size_t width, size_t height, size_t depth, 
                 size_t row_pitch, size_t slice_pitch, 
                 const cl_image_format *format, void *host_ptr, 
                 cl_mem_flags flags, cl_int *errcode_ret)
: MemObject(ctx, MemObject::Image3D, flags, host_ptr, errcode_ret),
  p_width(width), p_height(height), p_depth(depth), p_row_pitch(row_pitch),
  p_slice_pitch(slice_pitch), p_format(*format)
{
    
}

size_t Image3D::width() const
{
    return p_width;
}

size_t Image3D::height() const
{
    return p_height;
}

size_t Image3D::depth() const
{
    return p_depth;
}

size_t Image3D::row_pitch() const
{
    return p_row_pitch;
}

size_t Image3D::slice_pitch() const
{
    return p_slice_pitch;
}

cl_image_format Image3D::format() const
{
    return p_format;
}
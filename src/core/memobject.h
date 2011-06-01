#ifndef __MEMOBJECT_H__
#define __MEMOBJECT_H__

#include <CL/cl.h>

namespace Coal
{

class DeviceBuffer;
class Context;
class DeviceInterface;

class MemObject
{
    public:
        enum Type
        {
            Buffer,
            SubBuffer,
            Image2D,
            Image3D
        };
        
        /**
         * @note Doesn't do any initialization here, but in init. We only fill
         *       the private variables and check the values passed in argument.
         * @sa init
         */
        MemObject(Context *ctx, Type type, cl_mem_flags flags, void *host_ptr,
                  cl_int *errcode_ret);
        ~MemObject();
        
        virtual cl_int init();
        
        void reference();
        bool dereference();
        
        Type type() const;
        Context *context() const;
        cl_mem_flags flags() const;
        void *host_ptr() const;
        DeviceBuffer *deviceBuffer(DeviceInterface *device) const;
        
        void setDestructorCallback(void (CL_CALLBACK *pfn_notify)(cl_mem memobj,
                                                             void *user_data),
                                   void *user_data);
        
    private:
        Type p_type;
        Context *p_ctx;
        unsigned int p_references, p_num_devices;
        cl_mem_flags p_flags;
        void *p_host_ptr;
        DeviceBuffer **p_devicebuffers;
        
        void (CL_CALLBACK *p_dtor_callback)(cl_mem memobj, void *user_data);
        void *p_dtor_userdata;
};

class Buffer : public MemObject
{
    public:
        Buffer(Context *ctx, size_t size, void *host_ptr, cl_mem_flags flags, 
               cl_int *errcode_ret);
        
        size_t size() const;
    private:
        size_t p_size;
        
};

class SubBuffer : public MemObject
{
    public:
        SubBuffer(class Buffer *parent, size_t offset, size_t size, 
                  cl_mem_flags flags, cl_int *errcode_ret);
        
        size_t size() const;
        size_t offset() const;
        class Buffer *parent() const;
        
    private:
        size_t p_offset, p_size;
        class Buffer *p_parent;
};

class Image2D : public MemObject
{
    public:
        Image2D(Context *ctx, size_t width, size_t height, size_t row_pitch, 
                const cl_image_format *format, void *host_ptr, 
                cl_mem_flags flags, cl_int *errcode_ret);
        
        size_t width() const;
        size_t height() const;
        size_t row_pitch() const;
        cl_image_format format() const;
        
    private:
        size_t p_width, p_height, p_row_pitch;
        cl_image_format p_format;
};

class Image3D : public MemObject
{
    public:
        Image3D(Context *ctx, size_t width, size_t height, size_t depth, 
                size_t row_pitch, size_t slice_pitch, 
                const cl_image_format *format, void *host_ptr, 
                cl_mem_flags flags, cl_int *errcode_ret);
        
        size_t width() const;
        size_t height() const;
        size_t depth() const;
        size_t row_pitch() const;
        size_t slice_pitch() const;
        cl_image_format format() const;
        
    private:
        size_t p_width, p_height, p_depth, p_row_pitch, p_slice_pitch;
        cl_image_format p_format;
};

}

struct _cl_mem : public Coal::MemObject
{};

#endif
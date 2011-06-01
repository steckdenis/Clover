#include "test_mem.h"
#include "CL/cl.h"

#include <stdio.h>

START_TEST (test_create_buffer)
{
    cl_context ctx;
    cl_mem buf;
    cl_int result;
    char s[] = "Hello, world !";
    
    ctx = clCreateContextFromType(0, CL_DEVICE_TYPE_CPU, 0, 0, &result);
    fail_if(
        result != CL_SUCCESS,
        "unable to create a valid context"
    );
    
    buf = clCreateBuffer(0, CL_MEM_READ_WRITE, sizeof(s), 0, &result);
    fail_if(
        result != CL_INVALID_CONTEXT,
        "0 is not a valid context"
    );
    
    buf = clCreateBuffer(ctx, 1337, sizeof(s), 0, &result);
    fail_if(
        result != CL_INVALID_VALUE,
        "1337 is not a valid cl_mem_flags"
    );
    
    buf = clCreateBuffer(ctx, CL_MEM_USE_HOST_PTR, sizeof(s), 0, &result);
    fail_if(
        result != CL_INVALID_HOST_PTR,
        "host_ptr cannot be NULL if flags is CL_MEM_USE_HOST_PTR"
    );
    
    buf = clCreateBuffer(ctx, CL_MEM_COPY_HOST_PTR, sizeof(s), 0, &result);
    fail_if(
        result != CL_INVALID_HOST_PTR,
        "host_ptr cannot be NULL if flags is CL_MEM_COPY_HOST_PTR"
    );
    
    buf = clCreateBuffer(ctx, 0, sizeof(s), s, &result);
    fail_if(
        result != CL_INVALID_HOST_PTR,
        "host_ptr must be NULL if flags is not CL_MEM_{COPY/USE}_HOST_PTR"
    );
    
    buf = clCreateBuffer(ctx, CL_MEM_USE_HOST_PTR, 0, s, &result);
    fail_if(
        result != CL_INVALID_BUFFER_SIZE,
        "size cannot be 0"
    );
    
    buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                         sizeof(s), s, &result);
    fail_if(
        result != CL_SUCCESS,
        "cannot create a valid CL_MEM_COPY_HOST_PTR read-write buffer"
    );
    
    clReleaseMemObject(buf);
    clReleaseContext(ctx);
}
END_TEST

START_TEST (test_create_sub_buffer)
{
    cl_context ctx;
    cl_mem buf, subbuf;
    cl_int result;
    char s[] = "Hello, world !";
    
    cl_buffer_region create_info = {    // "Hello, [world] !"
        .origin = 7,
        .size = 5
    };
    
    ctx = clCreateContextFromType(0, CL_DEVICE_TYPE_CPU, 0, 0, &result);
    fail_if(
        result != CL_SUCCESS,
        "unable to create a valid context"
    );
    
    buf = clCreateBuffer(ctx, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
                         sizeof(s), s, &result);
    fail_if(
        result != CL_SUCCESS,
        "cannot create a valid CL_MEM_USE_HOST_PTR read-write buffer"
    );
    
    subbuf = clCreateSubBuffer(0, CL_MEM_WRITE_ONLY, 
                               CL_BUFFER_CREATE_TYPE_REGION, 
                               (void *)&create_info, &result);
    fail_if(
        result != CL_INVALID_MEM_OBJECT,
        "0 is not a valid mem object"
    );
    
    subbuf = clCreateSubBuffer(buf, CL_MEM_READ_ONLY,
                               CL_BUFFER_CREATE_TYPE_REGION,
                               (void *)&create_info, &result);
    fail_if(
        result != CL_INVALID_VALUE,
        "READ_ONLY is not compatible with WRITE_ONLY"
    );
    
    subbuf = clCreateSubBuffer(buf, CL_MEM_WRITE_ONLY, 1337, (void *)&create_info,
                            &result);
    fail_if(
        result != CL_INVALID_VALUE,
        "1337 is not a valid buffer_create_type"
    );
    
    subbuf = clCreateSubBuffer(buf, CL_MEM_WRITE_ONLY, 
                            CL_BUFFER_CREATE_TYPE_REGION, 0, &result);
    fail_if(
        result != CL_INVALID_VALUE,
        "buffer_create_info cannot be NULL"
    );
    
    create_info.size = 0;
    
    subbuf = clCreateSubBuffer(buf, CL_MEM_WRITE_ONLY,
                            CL_BUFFER_CREATE_TYPE_REGION,
                            (void *)&create_info, &result);
    fail_if(
        result != CL_INVALID_BUFFER_SIZE,
        "create_info.size cannot be 0"
    );
    
    create_info.size = 5;
    
    subbuf = clCreateSubBuffer(buf, CL_MEM_WRITE_ONLY,
                            CL_BUFFER_CREATE_TYPE_REGION,
                            (void *)&create_info, &result);
    fail_if(
        result != CL_SUCCESS || subbuf == 0,
        "cannot create a valid sub-buffer"
    );
    
    subbuf = clCreateSubBuffer(subbuf, CL_MEM_WRITE_ONLY,
                            CL_BUFFER_CREATE_TYPE_REGION,
                            (void *)&create_info, &result);
    fail_if(
        result != CL_INVALID_MEM_OBJECT,
        "we cannot create a sub-buffer of a sub-buffer"
    );
    
    clReleaseMemObject(subbuf);
    clReleaseMemObject(buf);
    clReleaseContext(ctx);
}
END_TEST

TCase *cl_mem_tcase_create(void)
{
    TCase *tc = NULL;
    tc = tcase_create("mem");
    tcase_add_test(tc, test_create_buffer);
    tcase_add_test(tc, test_create_sub_buffer);
    return tc;
}

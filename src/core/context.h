#ifndef CONTEXT_H
#define CONTEXT_H

#include "CL/cl.h"
#include "pipe/p_context.h"

struct _cl_context {
    struct pipe_context pipe;
    cl_uint id;
};

void cl_set_current_context( struct _cl_context *ctx);
struct _cl_context *cl_current_context( void);

struct pipe_context *cl_create_context();

static INLINE struct _cl_context *
cl_convert_context( struct pipe_context *pipe )
{
    return (struct _cl_context *)pipe;
}

#endif

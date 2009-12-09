#include "context.h"
#include "util/u_memory.h"

void cl_destroy_context( struct pipe_context *context )
{
    struct _cl_context *clcontext = cl_convert_context(context);

    FREE(clcontext);
}

struct pipe_context *cl_create_context( struct pipe_winsys *winsys )
{
    struct _cl_context *cl_context = CALLOC_STRUCT(_cl_context);

    cl_context->pipe.winsys = winsys;
    cl_context->pipe.destroy = cl_destroy_context;

    return &cl_context->pipe;
}

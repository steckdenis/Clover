#include "cpuwinsys.h"

#include "pipe/internal/p_winsys_screen.h"
#include "pipe/p_format.h"
#include "pipe/p_context.h"
#include "pipe/p_inlines.h"
#include "util/u_math.h"
#include "util/u_memory.h"

#ifdef GALLIUM_CELL
#include "cell/ppu/cell_context.h"
#include "cell/ppu/cell_screen.h"
#include "cell/ppu/cell_winsys.h"
#else
#define TILE_SIZE 32  /* avoid compilation errors */
#endif


/**
 * Subclass of pipe_winsys for Xlib winsys
 */
struct cpu_winsys {
   struct pipe_winsys base;
};

/**
 * Subclass of pipe_buffer for CL winsys.
 * Low-level OS/window system memory buffer
 */
struct cpu_buffer
{
   struct pipe_buffer base;
   boolean userBuffer;  /** Is this a user-space buffer? */
   void *data;
   void *mapped;
};


/** Cast wrapper */
static INLINE struct cpu_buffer *
cpu_buffer(struct pipe_buffer *buf)
{
   return (struct cpu_buffer *)buf;
}


/* Most callbacks map direcly onto dri_bufmgr operations:
 */
static void *
cpu_buffer_map(struct pipe_winsys *pws, struct pipe_buffer *buf,
               unsigned flags)
{
   struct cpu_buffer *cpu_buf = cpu_buffer(buf);
   cpu_buf->mapped = cpu_buf->data;
   return cpu_buf->mapped;
}

static void
cpu_buffer_unmap(struct pipe_winsys *pws, struct pipe_buffer *buf)
{
   struct cpu_buffer *cpu_buf = cpu_buffer(buf);
   cpu_buf->mapped = NULL;
}

static void
cpu_buffer_destroy(struct pipe_winsys *pws,
                   struct pipe_buffer *buf)
{
   struct cpu_buffer *oldBuf = cpu_buffer(buf);

   if (oldBuf->data) {
      if (!oldBuf->userBuffer) {
         align_free(oldBuf->data);
      }

      oldBuf->data = NULL;
   }

   free(oldBuf);
}


static void
cpu_flush_frontbuffer(struct pipe_winsys *pws,
                     struct pipe_surface *surf,
                     void *context_private)
{
   /*### do nothing? */
}



static const char *
cpu_get_name(struct pipe_winsys *pws)
{
   return "OpenCL CPU";
}


static struct pipe_buffer *
cpu_buffer_create(struct pipe_winsys *pws, 
                  unsigned alignment, 
                  unsigned usage,
                  unsigned size)
{
   struct cpu_buffer *buffer = CALLOC_STRUCT(cpu_buffer);

   buffer->base.alignment = alignment;
   buffer->base.usage = usage;
   buffer->base.size = size;

   if (buffer->data == NULL) {
      /* align to 16-byte multiple for Cell */
      buffer->data = align_malloc(size, MAX2(alignment, 16));
   }

   return &buffer->base;
}


/**
 * Create buffer which wraps user-space data.
 */
static struct pipe_buffer *
cpu_user_buffer_create(struct pipe_winsys *pws, void *ptr, unsigned bytes)
{
   struct cpu_buffer *buffer = CALLOC_STRUCT(cpu_buffer);
   buffer->base.size = bytes;
   buffer->userBuffer = TRUE;
   buffer->data = ptr;

   return &buffer->base;
}



/**
 * Round n up to next multiple.
 */
static INLINE unsigned
round_up(unsigned n, unsigned multiple)
{
   return (n + multiple - 1) & ~(multiple - 1);
}

static int
cpu_surface_alloc_storage(struct pipe_winsys *winsys,
                         struct pipe_surface *surf,
                         unsigned width, unsigned height,
                         enum pipe_format format,
                         unsigned flags,
                         unsigned tex_usage)
{
   const unsigned alignment = 64;

   surf->width = width;
   surf->height = height;
   surf->format = format;
   surf->usage = flags;

   return 0;
}


/**
 * Called via winsys->surface_alloc() to create new surfaces.
 */
static struct pipe_surface *
cpu_surface_alloc(struct pipe_winsys *ws)
{
   struct pipe_surface *surface = CALLOC_STRUCT(pipe_surface);

   assert(ws);

   return surface;
}



static void
cpu_surface_release(struct pipe_winsys *winsys, struct pipe_surface **s)
{
   struct pipe_surface *surf = *s;
   assert(!surf->texture);
   *s = NULL;
}


/*
 * Fence functions - basically nothing to do, as we don't create any actual
 * fence objects.
 */

static void
cpu_fence_reference(struct pipe_winsys *sws, struct pipe_fence_handle **ptr,
                   struct pipe_fence_handle *fence)
{
}


static int
cpu_fence_signalled(struct pipe_winsys *sws, struct pipe_fence_handle *fence,
                   unsigned flag)
{
   return 0;
}


static int
cpu_fence_finish(struct pipe_winsys *sws, struct pipe_fence_handle *fence,
                unsigned flag)
{
   return 0;
}

struct pipe_winsys * cpu_winsys(void)
{
   static struct cpu_winsys *ws = NULL;

   if (!ws) {
      ws = CALLOC_STRUCT(cpu_winsys);

      /* Fill in this struct with callbacks that pipe will need to
       * communicate with the buffer manager, etc.
       */
      ws->base.buffer_create = cpu_buffer_create;
      ws->base.user_buffer_create = cpu_user_buffer_create;
      ws->base.buffer_map = cpu_buffer_map;
      ws->base.buffer_unmap = cpu_buffer_unmap;

      ws->base.fence_reference = cpu_fence_reference;
      ws->base.fence_signalled = cpu_fence_signalled;
      ws->base.fence_finish = cpu_fence_finish;

      ws->base.flush_frontbuffer = cpu_flush_frontbuffer;
      ws->base.get_name = cpu_get_name;
   }

   return &ws->base;
}

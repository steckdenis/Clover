#ifndef CPUWINSYS_H
#define CPUWINSYS_H

#ifdef __cplusplus
extern "C" {
#endif

struct pipe_winsys;

struct pipe_winsys *cpu_winsys(void);

#ifdef __cplusplus
}
#endif

#endif

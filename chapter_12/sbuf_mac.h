#ifndef __SBUF_H__
#define __SBUF_H__

#include "csapp.h"
#include "macos_helpers.h"

/* $begin sbuft */
typedef struct {
    int *buf;                   /* Buffer array */
    int n;                      /* Maximum number of slots */
    int front;                  /* buf[(front+1)%n] is first item */
    int rear;                   /* buf[rear%n] is last item */
    dispatch_semaphore_t mutex; /* Protects accesses to buf */
    dispatch_semaphore_t slots; /* Counts available slots */
    dispatch_semaphore_t items; /* Counts available items */
} sbuf_t;
/* $end sbuft */

void sbuf_init(sbuf_t *sp, int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item);
int sbuf_remove(sbuf_t *sp);
int sbuf_lock_and_remove(sbuf_t *sp, dispatch_semaphore_t extra_mutex);
int sbuf_size(sbuf_t *sp);

#endif /* __SBUF_H__ */

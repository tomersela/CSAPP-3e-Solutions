/* $begin sbufc */
#include "csapp.h"
#include "sbuf_mac.h"
#include "macos_helpers.h"

/* Create an empty, bounded, shared FIFO buffer with n slots */
/* $begin sbuf_init */
void sbuf_init(sbuf_t *sp, int n)
{
    sp->buf = Calloc(n, sizeof(int));
    sp->n = n;                /* Buffer holds max of n items */
    sp->front = sp->rear = 0; /* Empty buffer iff front == rear */

    sp->mutex = dispatch_semaphore_create(1);
    sp->slots = dispatch_semaphore_create(n);
    sp->items = dispatch_semaphore_create(0);
}
/* $end sbuf_init */

/* Clean up buffer sp */
/* $begin sbuf_deinit */
void sbuf_deinit(sbuf_t *sp)
{
    Free(sp->buf);
}
/* $end sbuf_deinit */

/* Insert item onto the rear of shared buffer sp */
/* $begin sbuf_insert */
void sbuf_insert(sbuf_t *sp, int item)
{
    P_dispatch(sp->slots);                 /* Wait for available slot */
    P_dispatch(sp->mutex);                 /* Lock the buffer */
    sp->buf[(++sp->rear) % (sp->n)] = item; /* Insert the item */
    V_dispatch(sp->mutex);                 /* Unlock the buffer */
    V_dispatch(sp->items);                 /* Announce available item */
}
/* $end sbuf_insert */

/* Remove and return the first item from buffer sp */
/* $begin sbuf_remove */
int sbuf_remove(sbuf_t *sp)
{
    int item;
    P_dispatch(sp->items);                  /* Wait for available item */
    // lock specific thread mutex
    P_dispatch(sp->mutex);                  /* Lock the buffer */
    item = sp->buf[(++sp->front) % (sp->n)]; /* Remove the item */
    V_dispatch(sp->mutex);                  /* Unlock the buffer */
    // unlock specific thread mutex
    V_dispatch(sp->slots);                  /* Announce available slot */
    return item;
}
/* $end sbuf_remove */

int sbuf_size(sbuf_t *sp)
{
    int size;
    P_dispatch(sp->mutex);                 /* Lock the buffer */
    size = sp->rear - sp->front;
    V_dispatch(sp->mutex);                 /* Unlock the buffer */
    return size;
}

/* $end sbufc */

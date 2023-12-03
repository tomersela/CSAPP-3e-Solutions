#ifndef __CTHREADP_H__
#define __CTHREADP_H__

#include <pthread.h>
#include "macos_helpers.h"
#include "csapp.h"
#include "sbuf_mac.h"

#define MAX_THREADS 128 // By default MacOS BigSure throws "too many open files" exception in case more than 256 files open concurrently

typedef void (*conn_handler)(int);

typedef struct {
    sbuf_t conn_buf;                                // buffer of connected descriptors
    conn_handler connection_handler;                // connection handler function
    int thread_cnt;                                 // number of active threads
    dispatch_semaphore_t house_keeping_mutex;       // mutex for thread house-keeping activities (adding/remove threads from pool)
    dispatch_semaphore_t thread_mutex[MAX_THREADS]; // semaphore for each thread
    int deletion_mark[MAX_THREADS];                 // semaphore for each thread
} conn_thread_pool;

void thread_pool_init(conn_thread_pool *pool, conn_handler func);
void thread_pool_new_connection(conn_thread_pool *p, int connfd);

#endif /* __CTHREADP_H__ */
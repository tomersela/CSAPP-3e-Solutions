#include "conn_thread_pool.h"
#include "csapp.h"

#define INITIAL_THREAD_COUNT 8
#define SBUFSIZE 16

typedef struct {
    conn_thread_pool *pool;
    int thread_index;
} thread_pool_params;

static void *increase_threads(conn_thread_pool *pool);
static void *threadfunc(void *vargp);

void thread_pool_init(conn_thread_pool *pool, conn_handler func) {
    pthread_t tid;

    pool->thread_cnt = INITIAL_THREAD_COUNT;
    sbuf_init(&pool->conn_buf, SBUFSIZE); // Initialize connection descriptors buffer
    pool->connection_handler = func;
    pool->house_keeping_mutex = dispatch_semaphore_create(1);
    
    for (int i = 0; i < MAX_THREADS; i++)
        pool->thread_mutex[i] = dispatch_semaphore_create(1);
    
    for (int i = 0; i < MAX_THREADS; i++)
        pool->deletion_mark[i] = 0;
    
    for (int i = 0; i < INITIAL_THREAD_COUNT; i++) {
        thread_pool_params *params = Malloc(sizeof(thread_pool_params));
        params->pool = pool;
        params->thread_index = i;
        Pthread_create(&tid, NULL, threadfunc, (void *) params);
    }
}

void thread_pool_new_connection(conn_thread_pool *pool, int connfd) {
    P_dispatch(pool->house_keeping_mutex); // Make sure we're not racing with thread-reduction activity

    // Double the number of threads if the connection buffer is full
    int buf_size = sbuf_size(&pool->conn_buf);
    int is_buffer_full = (buf_size == pool->conn_buf.n);
    if (is_buffer_full && pool->thread_cnt < MAX_THREADS) { // connection buffer is about to be full
        increase_threads(pool);
    }

    V_dispatch(pool->house_keeping_mutex);

    sbuf_insert(&pool->conn_buf, connfd); // add new connection to the queue
}

static void *increase_threads(conn_thread_pool *pool) { // double the number of threads
    int double_cnt = pool->thread_cnt * 2;
    pthread_t tid;
        
    for (int i = pool->thread_cnt; i < double_cnt; i++) {
        pool->thread_mutex[i] = dispatch_semaphore_create(1);
        pool->deletion_mark[i] = 0;
        thread_pool_params *params = Malloc(sizeof(thread_pool_params));
        params->pool = pool;
        params->thread_index = i;
        Pthread_create(&tid, NULL, threadfunc, (void *) params); // initialize a new thread
    }

    printf("doubled the number of threads from %d to %d\n", pool->thread_cnt, double_cnt);
    pool->thread_cnt = double_cnt;
    return NULL;
}

static void *threadfunc(void *vargp) {
    Pthread_detach(Pthread_self());
    thread_pool_params* params = (thread_pool_params *) vargp;
    conn_thread_pool* pool = params->pool;
    int index = params->thread_index;

    while (1) {
        int connfd = sbuf_remove(&pool->conn_buf); // wait for a new connection
        
        P_dispatch(pool->thread_mutex[index]); // lock this thread mutex to protect from deletion
        if (pool->deletion_mark[index]) { // is this thread marked for deletion?
            sbuf_insert(&pool->conn_buf, connfd); // re-enqueue connection to the buffer (some other thread will handle)
            Pthread_exit(0); // bye bye!
        }
        V_dispatch(pool->thread_mutex[index]);
        
        pool->connection_handler(connfd);
        Close(connfd);
        
        //  House keeping! - If the connection buffer is empty when this thread leaves  ->
        //      do some house keeping and halve the threads in the pool
        P_dispatch(pool->house_keeping_mutex);
        int buf_size = sbuf_size(&pool->conn_buf);
        if (buf_size == 0 && pool->thread_cnt > INITIAL_THREAD_COUNT) { /*  connection buffer is empty
                                                                            and we have more threads then the initial count */
            // halve the number of threads
            int half_cnt = pool->thread_cnt / 2;
            for (int i = half_cnt; i < pool->thread_cnt; i++) {
                P_dispatch(pool->thread_mutex[i]);
                pool->deletion_mark[i] = 1; // mark thread for deletion
                V_dispatch(pool->thread_mutex[i]);
            }
            printf("reduced the number of threads from %d to %d\n", pool->thread_cnt, half_cnt);
            pool->thread_cnt = half_cnt;
        }
        V_dispatch(pool->house_keeping_mutex);
    }
    
    return NULL;
}

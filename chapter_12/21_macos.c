#include "csapp.h"
#include "macos_helpers.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvoid-pointer-to-int-cast"
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"

void init();
void *reader(void *vargp);
void *writer(void *vargp);

void test_simultaneous_reads();
void test_favour_writers();
void test_writer_waits_for_reader_to_finish();

/* Global variables */
int readcnt, writecnt;                /* Initially = 0 */
dispatch_semaphore_t r, w, mutex; /* All initially = 1 */

int shared;

int main(int argc, char *arg[])
{
    // reads waits until writes are done
    init();
    test_simultaneous_reads();
    test_favour_writers();
    test_writer_waits_for_reader_to_finish();

    return 0;
}

void init()
{
    readcnt = 0;
    writecnt = 0;
    r = dispatch_semaphore_create(1);
    w = dispatch_semaphore_create(1);
    mutex = dispatch_semaphore_create(1);

    shared = 0;
}

void *reader(void *vargp)
{
    int *i = (int *)vargp;
    char timebuf[26];
    int millisec;

    P_dispatch(r); // This will block in case a write operation is pending or ongoing
    V_dispatch(r);

    P_dispatch(mutex);
    readcnt++;
    if (readcnt == 1)
        P_dispatch(w); /* Lock writes */
    V_dispatch(mutex);

    /* Critical section */
    /* Reading happens */
    timeStr(timebuf, &millisec);
    printf("%s.%03d: ", timebuf, millisec);
    printf("I'm reading %d!\n", *i);
    Sleep(1);
    printf("Done reading %d!\n", *i);

    P_dispatch(mutex);
    readcnt--;
    if (readcnt == 0)
        V_dispatch(w); /* Unlock writes */
    V_dispatch(mutex);

    return NULL;
}

void *writer(void *vargp)
{
    int *i = (int *)vargp;
    char timebuf[26];
    int millisec;

    P_dispatch(mutex);
    writecnt++;
    if (writecnt == 1) // First in
        P_dispatch(r); // block reads
    V_dispatch(mutex);

    P_dispatch(w); /* Lock writes */

    // /* Critical section */
    // /* Writing happens */
    timeStr(timebuf, &millisec);
    printf("%s.%03d: ", timebuf, millisec);
    printf("Started writing %d!\n", *i);
    Sleep(1);
    printf("Done writing %d!\n", *i);

    V_dispatch(w); /* Unlock writes */

    P_dispatch(mutex);
    writecnt--;
    if (writecnt == 0)
        V_dispatch(r); // release reads
    V_dispatch(mutex);

    return NULL;
}

void test_simultaneous_reads()
{
    printf("\ntest_simultaneous_reads:\n");

    /*
    Test description:
    Here we test that multiple readers can work simultenously, but when there's a writer running, only it run at the same time.

    Flow:
    We'll create 10 threads - alternating between a writer thread and a reader thread for 5 times (5 writers and 5 readers in total).

    Each thread is going to print a string including a timestamp and sleep for 1 second.

    Expected results:
    All writer threads runs one after the other
    All readers wait for the writer threads to finish and then run simultaneous
    By looking at the timestamp we should be able to confirm the above expected behavior:
    - writes will run first with ±1 sec in between
    - readers will run with a short time-difference between them

    */

    int N = 10;
    pthread_t tid[N];
    int nums[N];
    for (int i = 0; i < N; i++)
    {
        nums[i] = i;
        Pthread_create(&tid[i], NULL, i % 2 ? reader : writer, &nums[i]);
    }

    for (int i = 0; i < N; i++)
        Pthread_join(tid[i], NULL);

    printf("\n");
}

void test_favour_writers()
{
    printf("\ntest_favour_writers:\n");

    /*
    Test description:
    While there are active read operations, we should have writes waiting

    Flow:
    1. create 2 reader threads
    2. create writer thread while the read threads are running
    3. create another reader thread while all the above are still running
    4. create a second writer thread while all the above are still running

    Expected results:
    Since the two reader threads already started, we should expect them waiting till completion.
    As soon as there's a writer thread waiting, we should make all additional reader executions wait until this writer is done working.
    So the expected sequence is:
    - first 2 reader threads (1) complete
    - then the first writer thread (2) completes
    - then the second writer thread (4) completes
    - then the last reader thread (3) completes.
    */

    int N = 5;

    pthread_t tid[N];
    // 1
    Pthread_create(&tid[0], NULL, reader, &(int){11});
    Pthread_create(&tid[1], NULL, reader, &(int){12});
    // 2
    Pthread_create(&tid[2], NULL, writer, &(int){2}); // this should wait until the first two reads complete
    // 3
    Pthread_create(&tid[3], NULL, reader, &(int){3}); // this should wait until all writes complete, means it should come last
    // 4
    Pthread_create(&tid[4], NULL, writer, &(int){4}); // this should get precedence over the previous write thread (3)

    for (int i = 0; i < N; i++)
        Pthread_join(tid[i], NULL);

    printf("\n");
}

void test_writer_waits_for_reader_to_finish()
{
    printf("\ntest_writer_waits_for_reader_to_finish:\n");

    /*
    Test description:
    Writers should wait for readers to finsish the critical section processing before starting executing their critical section.

    Flow:
    We're going to initialize a reader and right afer initialize a writer.

    Expected results:
    we should see the writer starts only once the reader is done.
    */

    int N = 2;

    pthread_t tid[N];
    Pthread_create(&tid[0], NULL, reader, &(int){1});
    Pthread_create(&tid[1], NULL, writer, &(int){2});

    for (int i = 0; i < N; i++)
        Pthread_join(tid[i], NULL);

    printf("\n");
}

#pragma clang diagnostic pop

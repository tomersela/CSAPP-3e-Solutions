#include <dispatch/dispatch.h>
#include <mach/mach_time.h>
#include "csapp.h"
#include "assert.h"

void P_dispatch(dispatch_semaphore_t sem)
{
    if (dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER) < 0)
        unix_error("P error");
}

void V_dispatch(dispatch_semaphore_t sem)
{
    if (dispatch_semaphore_signal(sem) < 0)
        unix_error("V error");
}

void timeStr(char buffer[26], int *ms)
{
    int millisec;
    struct tm *tm_info;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    millisec = lrint(tv.tv_usec / 1000.0); // Round to nearest millisec
    if (millisec >= 1000)
    { // Allow for rounding up to nearest second
        millisec -= 1000;
        tv.tv_sec++;
    }

    tm_info = localtime(&tv.tv_sec);

    strftime(buffer, 26, "%H:%M:%S", tm_info);
    *ms = millisec;
}
#ifndef __MACHELPERS_H__
#define __MACHELPERS_H__

#include <dispatch/dispatch.h>

void P_dispatch(dispatch_semaphore_t sem);
void V_dispatch(dispatch_semaphore_t sem);
void timeStr(char buffer[26], int *ms);

#endif /* __MACHELPERS_H__ */

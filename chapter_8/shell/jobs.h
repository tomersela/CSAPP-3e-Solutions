#ifndef __JOBS_H__
#define __JOBS_H__

#include <sys/types.h>

#define MAX_JOBS 100

typedef int jid_t;

typedef enum { Running, Stopped } JobStatus;

typedef enum { Invalid, Pid, Jid } JobIdentifierType;

typedef struct
{
    JobIdentifierType type;
    int id;
} JobIdentifier;

typedef struct 
{
    jid_t jid;
    pid_t pid;
    char *name;
    JobStatus status;
} Job, *JobPtr;

void init_jobs();
JobIdentifier parse_job_id(char* str);
JobPtr new_job(pid_t pid, char* command);
void print_jobs();
void bring_job_to_foreground(JobIdentifier identifier);
void continue_bg_job(JobIdentifier identifier);
void run_as_foreground_process(pid_t pid, sigset_t mask);

void jobs_sigchild_handler(int sig);
void jobs_sigint_handler(int sig);
void jobs_sigstop_handler(int sig);

#endif

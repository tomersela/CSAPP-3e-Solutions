#include "csapp.h"
#include "jobs.h"
#include <stdio.h>
#include <string.h>

static volatile sig_atomic_t fg_pid;
static JobPtr jobs[MAX_JOBS];

static JobIdentifier INVALID_JOB_ID = { Invalid, -1 };

static inline int is_fg_pid(pid_t pid) {
    return fg_pid == pid;
}

static inline void set_fg_pid(pid_t pid) {
    fg_pid = pid;
}

static inline void unset_fg_pid() {
    fg_pid = 0;
}

static JobPtr find_job(pid_t pid) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i] != NULL && jobs[i]->pid == pid) {
            return jobs[i];
        }
    }
    return NULL;
}

static JobPtr get_job(jid_t jid) {
    if (jid < 0 || jid > MAX_JOBS) {
        return NULL;
    }
    return jobs[jid];
}

static int parse_positive_number(const char *str) {
    if (str == NULL || *str == '\0') {
        return -1;
    }

    char *str_ptr = str;
    while (*str_ptr) {
        if (!isdigit(*str)) {
            return -1; // Invalid string if a non-digit character is encountered
        }
        str_ptr++;
    }

    return atoi(str);
}

void init_jobs() {
    for (int i = 0; i < MAX_JOBS; ++i) {
        jobs[i] = NULL;
    }
}


JobIdentifier parse_job_id(char* str) {
    if (str == NULL) {
        return INVALID_JOB_ID;
    }

    JobIdentifierType id_type = (str[0] == '%') ? Jid : Pid;
    char* id_ptr = (id_type == Jid) ? str + 1 : str;
    int maybe_id = parse_positive_number(id_ptr);
    if (maybe_id == -1) {
        return INVALID_JOB_ID;
    }

    JobIdentifier job_id = { id_type, maybe_id };
    return job_id;
}

JobPtr new_job(pid_t pid, char* command) {
    sigset_t mask_all, prev_all;
    Sigfillset(&mask_all);
    Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); // block all signals while aquiring spot in jobs array

    int jid = 0;
    while (jid < MAX_JOBS && jobs[jid] != NULL) { // find new empty position in the array
        jid++;
    }
    
    if (jid >= MAX_JOBS) { // No job slot found
        fprintf(stderr, "Maximum jobs reached!\n");
        exit(EXIT_FAILURE);
    }
    
    JobPtr job = (JobPtr) malloc(sizeof(Job));
    char* name = (char*) malloc(sizeof(strlen(command) + 1));
    strcpy(name, command);

    if (job == NULL || name == NULL) {
        fprintf(stderr, "Job memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    job->jid = jid;
    job->pid = pid;
    job->status = Running;
    job->name = name;

    jobs[jid] = job;
    Sigprocmask(SIG_SETMASK, &prev_all, NULL); // unblock all signals
    
    return job;
}

static inline void free_job(JobPtr job) {
    free(job->name);
    free(job);
}

static void remove_job_by_pid(pid_t pid) {
    JobPtr job = find_job(pid);
    if (job != NULL) {
        jobs[job->jid] = NULL;
        free_job(job);
    }
}

void print_jobs() {
    sigset_t mask_all, prev_all;
    Sigfillset(&mask_all);
    Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); // block all signals while aquiring spot in jobs array
    
    for (int i = 0; i < MAX_JOBS; i++) {
        JobPtr job = jobs[i];
        if (job != NULL) {
            char* status_str = job->status == Running ? "Running" : "Stopped";
            printf("[%d] %d %s \t %s\n", job->jid, job->pid, status_str, job->name);
        }
    }

    Sigprocmask(SIG_SETMASK, &prev_all, NULL); // unblock all signals
}

void run_as_foreground_process(pid_t pid, sigset_t mask) {
    fg_pid = pid;

    while(fg_pid) {
        sigsuspend(&mask);
    }
}

void bring_job_to_foreground(JobIdentifier identifier) {
    // Suspend signals from child processes until we're done processing the forground process
    sigset_t mask_sigchld, prev;
    Sigemptyset(&mask_sigchld);
    Sigaddset(&mask_sigchld, SIGCHLD);
    Sigprocmask(SIG_BLOCK, &mask_sigchld, &prev); // block SIGCHLD

    JobPtr job = identifier.type == Jid ? get_job(identifier.id) : find_job(identifier.id);
    if (job == NULL) {
        Sio_puts("Process not found\n");
        return;
    }

    set_fg_pid(job->pid); // Set forground process id
    Kill(job->pid, SIGCONT); // restarts job by sending it a SIGCONT signal

    while (fg_pid) { // while there's still forground process running
        sigsuspend(&prev); // suspend shell process (while temporarily unblock SIGCHLD)
    }
    
    Sigprocmask(SIG_SETMASK, &prev, NULL); // resume SIGCHLD handling
}

void continue_bg_job(JobIdentifier identifier) {
    // Suspend signals from child processes until we're done processing the forground process
    sigset_t mask_sigchld, prev;
    Sigemptyset(&mask_sigchld);
    Sigaddset(&mask_sigchld, SIGCHLD);
    Sigprocmask(SIG_BLOCK, &mask_sigchld, &prev); // block SIGCHLD

    JobPtr job = identifier.type == Jid ? get_job(identifier.id) : find_job(identifier.id);
    if (job == NULL) {
        Sio_puts("Process not found\n");
        return;
    }
    
    job->status = Running;
    Kill(job->pid, SIGCONT); // restart job by sending it a SIGCONT signal

    Sigprocmask(SIG_SETMASK, &prev, NULL); // resume SIGCHLD handling
}

void jobs_sigchild_handler(int sig) {
    
    int old_errno = errno;
    int status;
    pid_t pid;

    sigset_t mask_all, prev;
    Sigfillset(&mask_all);

    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) { // child process exited, stopped, continued
        Sigprocmask(SIG_BLOCK, &mask_all, &prev); // pause all signals

        if (WIFEXITED(status) || WIFSIGNALED(status)) { // child exited
            if (is_fg_pid(pid)) {
                unset_fg_pid(); // unset foreground process
            } else {
                Sio_puts("\nProcess ");
                Sio_putl(pid);
                Sio_puts(" terminated\n");
            }
            remove_job_by_pid(pid);
        }

        if (WIFSTOPPED(status)) { // child stopped
            if (is_fg_pid(pid)) { // foreground process stopped
                unset_fg_pid(); // unset foreground process
            }
            // set pid status stopped
            JobPtr jp = find_job(pid);
            jp->status = Stopped;

            Sio_puts("\nProcess ");
            Sio_putl(pid);
            Sio_puts(" stopped\n");
        }

        if(WIFCONTINUED(status)) { // continued process brought to the foreground
            set_fg_pid(pid);
            JobPtr jp = find_job(pid);
            jp->status = Running; // set pid status running

            Sio_puts("\nProcess ");
            Sio_putl(pid);
            Sio_puts(" continuing\n");
        }
        
        Sigprocmask(SIG_SETMASK, &prev, NULL); // resume all signals
    }
    
    errno = old_errno;
}

void jobs_sigint_handler(int sig) {
    int old_errno = errno;
    sigset_t mask_all, prev;
    Sigfillset(&mask_all);
    Sigprocmask(SIG_BLOCK, &mask_all, &prev); // pause all signals
    
    if (is_fg_pid(0)) { // if no foreground process, SIGINT was destined to the shell process
        Signal(SIGINT, SIG_DFL); // restore default SIGINT handler
        Kill(getpid(), SIGINT); // send SIGINT to self
    } else {
        Kill(fg_pid, SIGINT); // send SIGINT to foreground process
    }

    Sio_puts("\n");
    
    Sigprocmask(SIG_SETMASK, &prev, NULL); // resume all signals
    errno = old_errno;
}

void jobs_sigstop_handler(int sig) {
    int old_errno = errno;
    sigset_t mask_all, prev;
    Sigfillset(&mask_all);
    Sigprocmask(SIG_BLOCK, &mask_all, &prev); // pause all signals

    if (is_fg_pid(0)) { // if no foreground process, SIGTSTP was destined to the shell process
        Signal(SIGTSTP, SIG_DFL); // restore default SIGTSTP handler
        Kill(getpid(), SIGTSTP); // send SIGTSTP to self
    } else {
        Kill(fg_pid, SIGTSTP); // send SIGTSTP to foreground process
    }
    
    Sigprocmask(SIG_SETMASK, &prev, NULL); // resume all signals
    errno = old_errno;
}
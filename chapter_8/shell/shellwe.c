#include "csapp.h"
#include "jobs.h"
#define MAXARGS 128

void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);
void builtin_command_fg(char **argv);
void builtin_command_bg(char **argv);

int main() {

    init_jobs();

    /* set signal handlers */
    if (Signal(SIGCHLD, jobs_sigchild_handler) == SIG_ERR)
        unix_error("signal child handler error");
    if (Signal(SIGINT, jobs_sigint_handler) == SIG_ERR)
        unix_error("signal int handler error");
    if (Signal(SIGTSTP, jobs_sigstop_handler) == SIG_ERR)
        unix_error("signal stop handler error");

    char cmdline[MAXLINE]; /* Command line */

    while (1) {
        printf("> ");                   
        Fgets(cmdline, MAXLINE, stdin); 
        if (feof(stdin))
            exit(0);

        eval(cmdline);
    }
}

void eval(char *cmdline) 
{
    char *argv[MAXARGS]; // Argument list execve()
    char buf[MAXLINE];   // Holds modified command line
    int bg;              // Should the job run in bg or fg?
    pid_t pid;           // Process id
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	    return; // Ignore empty lines

    if (!builtin_command(argv)) {
        // Suspend signals from child processes
        sigset_t mask_sigchld, prev;
        Sigemptyset(&mask_sigchld);
        Sigaddset(&mask_sigchld, SIGCHLD);
        Sigprocmask(SIG_BLOCK, &mask_sigchld, &prev); // block SIGCHLD

        if ((pid = Fork()) == 0) { // Child runs user job
            Sigprocmask(SIG_SETMASK, &prev, NULL); // unblock SIGCHLD handling in child process
            
            Setpgid(0, 0); // set gid to be the same as pid

            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        JobPtr job = new_job(pid, cmdline);
        
        if (!bg) {
            run_as_foreground_process(job->pid, prev);
        } else
            printf("[%d] %d %s \t %s\n", job->jid, pid, "Running", cmdline);

        Sigprocmask(SIG_SETMASK, &prev, NULL); // resume SIGCHLD handling
    }
    return;
}

int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) // quit command
	    exit(0);

    if (!strcmp(argv[0], "&"))    // Ignore singleton &
	    return 1;

    if (!strcmp(argv[0], "jobs")) {
        print_jobs();
        return 1;
    }

    if (!strcmp(argv[0], "fg")) {
        builtin_command_fg(argv);
        return 1;
    }

    if (!strcmp(argv[0], "bg")) {
        builtin_command_bg(argv);
        return 1;
    }

    return 0;                     // Not a builtin command
}

int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	    buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	    argv[--argc] = NULL;

    return bg;
}

void builtin_command_fg(char **argv) {
    JobIdentifier identifier = parse_job_id(argv[1]);
    if (identifier.type == Invalid || argv[2] != NULL) {
        printf("Error: Invalid job id\n");
        return;
    }

    bring_job_to_foreground(identifier);
}

void builtin_command_bg(char **argv) {
    JobIdentifier identifier = parse_job_id(argv[1]);
    if (identifier.type == Invalid || argv[2] != NULL) {
        printf("Error: Invalid job id\n");
        return;
    }

    continue_bg_job(identifier);
}

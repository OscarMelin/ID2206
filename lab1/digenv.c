/*
NAME:

    digenv - view enviroment variablels in a sorted list w/ or w/o regex filter

SYNTAX:
    
    digenv [REGEX]

DESCRIPTION:

    digenv display the enviroment variables in sorted order using the default
    pager specified in the enviroment variables.
    If a regex pattern is provided, digenv will display a filtered version of
    the output described above.

OPTIONS:

    See grep(1)

EXAMPLES:

    digenv
    digenv "PATTERN" 
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int args;
int num_procs;
int num_pipes;
int pipes[3][2];
char *pager;


/* Initializes all required pipes */
void pipes_init() {

    int ret;
    int i;

    for (i = 0; i < num_procs - 1; i++) {

        ret = pipe(pipes[i]);

        if (ret == -1) {

            perror("pipe");
            exit(1);
        }
    }
}

/* Closes all pipe file descriptors */
void close_all_fd() {

    int i;
    int j;

    for (i = 0; i < num_pipes; i++) {
        
        for (j = 0; j < 2; j++) {
            
            int ret = close(pipes[i][j]);
            if (ret == -1) {
                
                perror("close");
                exit(-1);
            }
        }
    }
}

/* Waits for att child processes to finish */
void wait_for_kids() {

    int status;
    int i;

    for (i = 0; i < num_procs; i++) {
        
        /* Wait for process to finish */
        wait(&status);
		if(!WIFEXITED(status)) {
            
            /* Process exited abnormaly */
            perror("wait_for_kids");
            kill(0, SIGTERM);
            exit(-1);        
        }
    }
}

/*
Forks and executes the "program" with arguments args in a different process.
STDIN and STDOUT are redirected to fd_in and fd_out and used for the required
inter-process communication. 
*/
pid_t exec_proc(int fd_in, int fd_out, char *program, char *argv[]) {
    
    pid_t pid = fork();

    if (pid == 0) {
        
        /*Duplicate stdin and stdout file descriptors*/
        int ret;
        char *ptr[2];

        ret = dup2(fd_in, STDIN_FILENO);
        if (ret == -1) { 

            perror("exec_proc\n");
            exit(-1);
        }
        ret = dup2(fd_out, STDOUT_FILENO);
        if (ret == -1) {
            
            perror("exec_proc\n");
            exit(-1);
        }

        /* Close all file descriptors in pipes */
        close_all_fd();        
        
        /* NULL args for all programs but grep */
        ptr[0] = program;
        ptr[1] = (char*) NULL;
        
        /* Execute */
        if (!strcmp(program, "grep")) {
            
            execvp(program, argv);
            exit(-1);

        } else if (!strcmp(program, pager)) {

            int exec_ret;

            exec_ret = execvp(program, ptr);

            /* If pager fails, default to more */
            if (exec_ret == -1) {
                
                execvp("more", ptr);
            }

        } else {

            execvp(program, ptr);
            exit(-1);
        }

    } else if (pid == -1) {

        perror("exec_proc");
        exit(-1);
    }
    return pid;
}

int main(int argc, char *argv[]) {

    pager = getenv("PAGER");

    if (pager == NULL)
        pager = "less";

    /* Set number of processes and pipes depending on if we should run grep */
    args = (argc > 1) ? 1 : 0;
    num_procs = 3 + args;
    num_pipes = 2 + args;

    pipes_init();

    exec_proc(STDIN_FILENO, pipes[0][1], "printenv", argv); 

    if (args) {

        exec_proc(pipes[0][0], pipes[args][1], "grep", argv);
    }
    exec_proc(pipes[args][0], pipes[1 + args][1], "sort", argv);

    exec_proc(pipes[1 +args][0], STDOUT_FILENO, "less", argv);

    close_all_fd();

    wait_for_kids();

    return 0;
}

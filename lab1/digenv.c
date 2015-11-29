#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int args;
int num_procs;
int num_pipes;
int pipes[3][2];

void pipes_init() {

    int ret;
    int i;

    for (i = 0; i < num_procs - 1; ++i) {

        ret = pipe(pipes[i]);

        if (ret == -1) {

            perror("pipe");
            exit(1);
        }
    }
}

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

void wait_for_kids() {

    int status;
    pid_t pid;
    int i;

    for (i = 0; i < num_procs; i++) {
        
        /* Wait for process to finish */
        pid = wait(&status);
		if(!WIFEXITED(status)) {
            
            /* Process exited abnormaly */
            perror("wait_for_kids");
            /*TODO Some cleanup funtion call??*/            
            exit(-1);        
        }
    }
}

int exec_proc(int fd_in, int fd_out, char *program, char *argv[]) {
    
    int pid = fork();

    if (pid == 0) {
        
        printf("Forked: %s\n", program);
        /*Duplicate stdin and stdout file descriptors*/
        int ret;

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

        /*Close all file descriptors in pipes*/
        close_all_fd();        
        
        /*check if grep*/
        if (args) {
            
            execvp(program, argv);            
        } else {

            char *ptr[2] = {program, (char*) NULL};
            execvp(program, ptr);
        }

    } else if (pid == -1) {
        printf("exec_proc failed\n");
        perror("exec_proc");
        exit(-1);
    }
    return 0;
}

int main(int argc, char *argv[]) {

    char *pager = getenv("PAGER");

    if (pager == NULL)
        pager = "less";

    args = (argc > 1) ? 1 : 0;
    num_procs = 3 + args;
    num_pipes = 2 + args;

    /*Initialize pipes*/
    pipes_init();
    /* Set pager */

    /* Execute printenv */
    exec_proc(STDIN_FILENO, pipes[0][1], "printenv", argv); 

    /* Check and execute grep */
    if (args) {

        exec_proc(pipes[0][0], pipes[args][1], "grep", argv);
    }

    /* Execute sort */
    exec_proc(pipes[args][0], pipes[1 + args][1], "sort", argv);

    /* Execute less or more */
    exec_proc(pipes[1 +args][0], STDOUT_FILENO, "less", argv);

    /* Close all file descriptors*/
    close_all_fd();
    /* Wait for all child processes to terminate */
    wait_for_kids();
    return 0;
}

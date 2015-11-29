#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int args;
int num_procs;
int num_pipes;
int pipes[3][2];

void pipes_init() {

    int ret;
    int i;

    for(i = 0; i < num_procs - 1; ++i) {

        ret = pipe(pipes[i]);

        if(ret == -1) {
            perror("pipe");
            exit(1);
        }
    }
}

int exec_proc(int fd_in, int fd_out, char *program, char *argv[]) {

    int pid = fork();

    if (pid == 0) {




    } else if (pid == -1) {
        perror("exec_proc()");
        exit(-1);
    }

}

int main(int argc, char *argv[]) {

    char *pager = getenv("PAGER");

    if(pager == NULL)
        pager = "less";

    args = (argc > 1) ? 1 : 0;
    num_procs = 3 + args;
    num_pipes = 2 + args;

    /*Initialize pipes*/
    pipes_init();

    /* Set pager */

    /* Execute printenv */

    /* Check and execute grep */

    /* Execute sort */

    /* Execute less or more */

    /* Close all file descriptors*/

    /* Wait for all processes to finish */

    if( argc == 2 ) {

        printf("The argument supplied is %s\n", argv[1]);
    }
    else if( argc > 2 ) {

        printf("Too many arguments supplied.\n");
    }
    else {

        printf("One argument expected.\n");
    }
    return 0;
}

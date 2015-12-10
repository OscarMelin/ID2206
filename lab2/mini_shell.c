/*
NAME:

SYNTAX:

DESCRIPTION:

OPTIONS:

EXAMPLES:

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int exit_mini_shell = 0; /*1 if exit*/

/* Signal Handler for SIGINT */
void sigintHandler(int sig_num)
{
    /* Reset handler to catch SIGINT next time. */
    signal(SIGINT, sigintHandler);
    printf("\n Cannot be terminated using Ctrl+C \n");
    fflush(stdout);
}

pid_t exec_proc(char *program, char *params[], int background) {
    
    pid_t pid = fork();
    int pipes[2];
    int status;
    
    if (background) {

        pipe(pipes);    
    }

    pid = fork();

    if ( pid == 0 ) { /* child-process kod */

        if (background) {

            int ret;

            ret = dup2(pipes[STDIN_FILENO], STDIN_FILENO);
            if (ret == -1) { 

                perror("exec_proc");
                exit(-1);
            }
            close(pipes[0]);
            close(pipes[1]);
        }

        execvp(program, params);
		printf("%s: command not found", params[0]);
        exit(-1);

    } else if (pid == -1) {

        perror("exec_proc");
        exit(-1);
    }

    if (background) {
    
        /* Close pipes */
        close(pipes[0]);
        close(pipes[1]);

    } else {

        waitpid(pid, &status, 0);
    }
    return pid;
}

/*Returns 1 - if exit*/
int exec_program(char *str) {

    char *params[5];
    int i;

    /* Parse first parameter */
    params[0] = strtok(str, " \t\n");


    if (params[0] == NULL) {

        return 0;    
    }
    /* Parse the remaining parameters */
    for (i = 1; i < 5; i++) {

        params[i] = strtok(NULL, " \t\n");
    }

	if(!strcmp(params[0], "exit")) {
    
        exit_mini_shell = 1;
        return 1;


	} else if(!strcmp(params[0], "cd")) {

        /*Change directory of process with chdir(path)*/
        printf("cd command");

    /*  */
	} else {

    /* TODO: Check if & as last param, then run as background */

    /* Foreground */
        exec_proc(params[0], params, 0);
    }

    return 0;
}

int main(int argc, char *argv[]) {

    char str[72];
    char *input;
    
    signal(SIGINT, sigintHandler);

    for (;;) {

        input = NULL;

        for (;;) {

            fputs(">>> ", stdout);

            input = fgets(str, sizeof(str), stdin);

            if (exec_program(input)) {

                break;            
            }

        }
    
        if (exit_mini_shell) {
            
            /*Call some cleanup function here*/
            break;
        }
    }

    return 0;
}

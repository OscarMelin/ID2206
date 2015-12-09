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

pid_t exec_proc(char *program, char *params[], int background) {
    
    pid_t pid = fork();
    int pipes[2];
    int status;
    
    if (background) {

        pipe(pipes);    
    }

    pid = fork();

    if ( pid == 0 ) { /* child-process kod */

        execvp(program, params);
        exit(-1);
    }
    else if ( pid > 0) { /* parent-process kod */

        waitpid(pid, &status, 0);
    }
    else { /* SYSTEM ERROR */

        perror("exec_proc");
        exit(-1);
    }

    if (background) {
    
    /* Close pipes */
    } else {


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

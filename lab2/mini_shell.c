/*
NAME:

    mini_shell.c - Simple shell like program with little practical use

SYNTAX:

    [PROGRAM] [ARGS]

DESCRIPTION:

    mini_shell allows for running programs as either forground or background
    processes.

    Commands must not be longer than 70 characters and contain < 5 arguments.

    Built in functions include:

        cd [path] - changes working directory, if no path or an incorrect path is
        given; cd will change working directory to HOME.

        exit - exits shell

OPTIONS:

    Compile with flag -DSIGNALDETECTION to use signal handler, otherwise defaults
    to polling.

EXAMPLES:

    Compilation:
        gcc -pedantic -W -Wall -o mini_shell mini_shell.c
        gcc -pedantic -W -Wall -DSIGNALDETECTION -o mini_shell mini_shell.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

int exit_mini_shell = 0; /*1 if exit*/

/* Signal Handler for SIGINT */
void sigintHandler() {

    /* Reset handler to catch SIGINT next time. */
    signal(SIGINT, sigintHandler);
    printf("\n>>>");
    fflush(stdout);
}

#ifndef SIGNALDETECTION
/* Listens for terminated bg-processes */
void bg_sig_handler(int signal, pid_t fg_pid) {

    int status;

    for (;;) {

        pid_t pid = waitpid(-1, &status, WNOHANG);

        /* Nothing to wait for */
        if(pid <= 0) {
            break;
        }

        if(WIFEXITED(status) || WIFSIGNALED(status))
            printf("Terminated background pid: %d\n", pid);

        if(pid == fg_pid) {
            continue;
        }
    }
        return;
}
#endif

#ifdef SIGNALDETECTION
void handle_child(int signo) {

    pid_t pid;
    int status;

    if( signo == SIGCHLD ){

        while (0 < (pid = waitpid (-1, & status , WNOHANG))){

        printf("Terminated background pid: %d\n", pid);
        }
    }
}
#endif

int change_directory(char *path) {

    int ret;
    
    ret = chdir(path);

    if (ret == 0) {
        
        printf("cd to: %s\n", path);
        return 0;

    } else {
        
        ret = chdir(getenv("HOME"));       
    
        if (ret == 0) {

            fprintf(stderr, "%s empty or unaccessible, cd to HOME\n", path);
            return 0;

        } else {

            fprintf(stderr, "error: HOME unreachable\n");
            return -1;
        }
    }
}

pid_t exec_proc(char *program, char *params[], int background) {
    
    pid_t pid;
    int pipes[2];
    
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

		fprintf(stderr, "%s: command not found\n", params[0]);
        exit(-1);

    } else if (pid == -1) {

        perror("exec_proc");
        exit(-1);
    }
    return pid;
}

/*Returns 1 - if exit*/
int exec_program(char *str) {

    char *params[5];
    int i;
    int background;
    pid_t pid;
    int status;
    /* Timing vars */
    clock_t begin, end;

    /* Parse first parameter */
    params[0] = strtok(str, " \t\n");

    #ifndef SIGNALDETECTION
    bg_sig_handler(SIGCHLD, pid);
    #endif

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

        change_directory(params[1]);
    /*  */
	} else {

        /* Check if & as last param, then run as background */
        for (i = 4; i > 0; i--) {

            if (params[i]) {

                if (!strcmp(params[i], "&")) {

                    background = 1;
                } else {
                    
                    background = 0;
                }
                break;
            }
        }

        if (background == 1) {
            
                pid = exec_proc(params[0], params, 1);
                printf("Spawned background pid: %d\n", pid);

        } else {
        /* Foreground */

            signal(SIGINT, sigintHandler);

            begin = clock();

            pid = exec_proc(params[0], params, 0);
            printf("Spawned pid: %d\n", pid);
            waitpid(pid, &status, 0);

            end = clock();
            printf("Terminated pid: %d\n", pid);
            printf("Elapsed: %f seconds\n", (double)(end - begin) / CLOCKS_PER_SEC);

        }
    }
    return 0;
}

int main() {

    char str[72];
    char *input;

    #ifdef SIGNALDETECTION
    printf("Using SIGNALDETECTION\n");
    signal( SIGCHLD, handle_child);
    #endif
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
            
            break;
        }
    }

    return 0;
}

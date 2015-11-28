#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int args;
int num_procs;
int num_pipes;
int pipes[3][2];


int main(int argc, char *argv[]) {

    char *pager = getenv("PAGER");

    if(pager == NULL)
        pager = "less";

    args = (argc > 1) ? 1 : 0;
    num_procs = 3 + args;
    num_pipes = 2 + args;

    /*Initialize pipes*/


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

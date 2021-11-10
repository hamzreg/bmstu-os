#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define FORK_ERROR -1
#define ERROR       1
#define OK          0

#define PAUSE       4

#define TASK        "\n<<<<< Task 1 : Creating orphan processes >>>>>\n\n"


int main(void)
{
    printf(TASK);

    pid_t child_pid1, child_pid2;

    if ((child_pid1 = fork()) == FORK_ERROR)
    {
        perror("\nCan't fork child 1.\n");
        exit(ERROR);
    }
    else if (child_pid1 == 0)
    {
        printf("\nBEFORE: \
                \nChild 1: PID = %d, PPID = %d, GPID = %d.\n", getpid(), getppid(), getpgrp());

        sleep(PAUSE);

        printf("\nAFTER: \
                \nChild 1: PID = %d, PPID = %d, GPID = %d.\n", getpid(), getppid(), getpgrp());

        exit(OK);
    }
    else
    {
        printf("\nParent: PID = %d, GPID = %d, child 1 PID = %d.\n", getpid(), getpgrp(), child_pid1);
    }


    if ((child_pid2 = fork()) == FORK_ERROR)
    {
        perror("\nCan't fork child 2.\n");
        exit(ERROR);
    }
    else if (child_pid2 == 0)
    {
        printf("\nBEFORE: \
                \nChild 2: PID = %d, PPID = %d, GPID = %d.\n", getpid(), getppid(), getpgrp());

        sleep(PAUSE);

        printf("\nAFTER: \
                \nChild 2: PID = %d, PPID = %d, GPID = %d.\n", getpid(), getppid(), getpgrp());

        exit(OK);
    }
    else
    {
        printf("\nParent: PID = %d, GPID = %d, child 2 PID = %d.\n", getpid(), getpgrp(), child_pid2);
    }

    return OK;
}

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define OK    0
#define ERROR 1
#define PAUSE 4

int main(void)
{
    pid_t child_pid1 = fork();

    if (child_pid1 == -1)
    {
        perror("Can't fork child1.");
        return ERROR;
    }
    else if (child_pid1 == 0)
    {
        printf("\nBEFORE\n");
        printf("Child 1: PID = %d, PPID = %d, GPID = %d\n", getpid(), getppid(), getpgrp());

        sleep(PAUSE);

        printf("\nAFTER\n");
        printf("Child 1: PID = %d, PPID = %d, GPID = %d\n", getpid(), getppid(), getpgrp());

        return OK;
    }
    else
    {
        printf("\nParent: child 1 PID = %d, PID = %d, GPID = %d\n", child_pid1, getpid(), getpgrp());
    }

    pid_t child_pid2 = fork();

    if (child_pid2 == -1)
    {
        perror("Can't fork child2.");
        return ERROR;
    }
    else if (child_pid2 == 0)
    {
        printf("\nBEFORE\n");
        printf("Child 2: PID = %d, PPID = %d, GPID = %d\n", getpid(), getppid(), getpgrp());

        sleep(PAUSE);

        printf("\nAFTER\n");
        printf("Child 2: PID = %d, PPID = %d, GPID = %d\n", getpid(), getppid(), getpgrp());
        return OK;
    }
    else
    {
        printf("\nParent: child 2 PID = %d, PID = %d, GPID = %d\n", child_pid2, getpid(), getpgrp());
    }

    return OK;
}

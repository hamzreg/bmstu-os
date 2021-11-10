#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define OK    0
#define ERROR 1
#define PAUSE 3

void check_status(int status)
{
    if (WIFEXITED(status))
    {
        printf("Child exited with code %d\n", WEXITSTATUS(status));

        return;
    }
    else if (WIFSIGNALED(status))
    {
        printf("Child exited with code %d\n", WTERMSIG(status));

        return;
    }
    else if (WIFSTOPPED(status))
    {
        printf("Child exited with code %d\n", WSTOPSIG(status));

        return;
    }
}

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

        int status;
        pid_t child_pid = wait(&status);
        printf("\nChild 1 has fihished: child PID = %d, status = %d\n", child_pid, status);
        printf("Parent: PID = %d\n", getpid());
        check_status(status);
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

        int status;
        pid_t child_pid = wait(&status);
        printf("\nChild 2 has fihished: child PID = %d, status = %d\n", child_pid, status);
        printf("Parent: PID = %d\n", getpid());
        check_status(status);
    }

    return OK;
}
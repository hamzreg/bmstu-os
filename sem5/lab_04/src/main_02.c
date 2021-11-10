#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FORK_ERROR -1
#define ERROR       1
#define OK          0

#define PAUSE       4

#define TASK        "\n<<<<< Task 2 : Parent is waiting childs with wait() >>>>>\n\n"


void check_status(int status)
{
    /*
    *  Проверка статуса завершения
    *  процесса-потомка.
    */

    if (WIFEXITED(status))
    {
        printf("Child exited correctly with code %d.\n", WEXITSTATUS(status));

        return;
    }
    else if (WIFSIGNALED(status))
    {
        printf("Child exited with non-interceptable signal %d.\n", WTERMSIG(status));

        return;
    }
    else if (WIFSTOPPED(status))
    {
        printf("Child stopped with signal %d.\n", WSTOPSIG(status));

        return;
    }
}


int main(void)
{
    printf(TASK);

    pid_t child_pid1, child_pid2, child_pid;
    int status;

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
        child_pid = wait(&status);
        printf("\nChild 1 has fihished: PID = %d, status = %d.\n", child_pid, status);

        printf("\nParent: PID = %d, GPID = %d, child 1 PID = %d.\n", getpid(), getpgrp(), child_pid1);
        check_status(status);
    }


    if ((child_pid2 = fork()) == FORK_ERROR)
    {
        perror("\nCan't fork child 2.\n");
        exit(ERROR);
    }
    else if (child_pid2 == 0)
    {
        printf("\n\n\nBEFORE: \
                \nChild 2: PID = %d, PPID = %d, GPID = %d.\n", getpid(), getppid(), getpgrp());

        sleep(PAUSE);

        printf("\nAFTER: \
                \nChild 2: PID = %d, PPID = %d, GPID = %d.\n", getpid(), getppid(), getpgrp());

        exit(OK);
    }
    else
    {
        child_pid = wait(&status);
        printf("\nChild 1 has fihished: PID = %d, status = %d.\n", child_pid, status);

        printf("\nParent: PID = %d, GPID = %d, child 1 PID = %d.\n", getpid(), getpgrp(), child_pid1);
        check_status(status);
    }

    return OK;
}

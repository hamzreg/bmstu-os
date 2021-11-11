#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define FORK_ERROR -1
#define PIPE_ERROR -1
#define ERROR       1
#define OK          0


#define TASK        "\n<<<<< Task 4 : Messaging with pipe() >>>>>\n\n"

#define READ  0
#define WRITE 1

#define MSG1  "Hello, child 2.\n"
#define MSG2  "How are you, child 1?\n"
#define LEN   50


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

    int fd[2];

    pid_t child_pid1, child_pid2, child_pid;
    int status;

    char msgs[LEN] = {0};

    if (pipe(fd) == PIPE_ERROR)
    {
        perror("\nCan't pipe.\n");
        exit(ERROR);
    }


    if ((child_pid1 = fork()) == FORK_ERROR)
    {
        perror("\nCan't fork child 1.\n");
        exit(ERROR);
    }
    else if (child_pid1 == 0)
    {
        printf("\nChild 1: PID = %d, PPID = %d, GPID = %d.\n", getpid(), getppid(), getpgrp());

        close(fd[READ]);
        write(fd[WRITE], MSG1, strlen(MSG1));

        exit(OK);
    }


    if ((child_pid2 = fork()) == FORK_ERROR)
    {
        perror("\nCan't fork child 2.\n");
        exit(ERROR);
    }
    else if (child_pid2 == 0)
    {
        printf("Child 2: PID = %d, PPID = %d, GPID = %d.\n", getpid(), getppid(), getpgrp());

        close(fd[READ]);
        write(fd[WRITE], MSG2, strlen(MSG2));

        exit(OK);
    }


    child_pid = wait(&status);
    printf("\n\nChild 1 has fihished: PID = %d, status = %d.\n", child_pid, status);
    check_status(status);

    child_pid = wait(&status);
    printf("\nChild 2 has fihished: PID = %d, status = %d.\n", child_pid, status);
    check_status(status);

    close(fd[WRITE]);
    read(fd[READ], msgs, LEN);
    printf("\nChilds wrote :\n%s\n", msgs);

    return OK;
}

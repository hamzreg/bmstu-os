#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define OK    0
#define ERROR 1
#define PAUSE 4

#define READ  0
#define WRITE 1

#define MSG1  "HELLO, "
#define MSG2  "WORLD!"
#define LEN   20

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
    int fd[2];
    pipe(fd);

    pid_t child_pid1 = fork();

    if (child_pid1 == -1)
    {
        perror("Can't fork child1.");
        return ERROR;
    }
    else if (child_pid1 == 0)
    {
        printf("Child 1: PID = %d, PPID = %d, GPID = %d\n", getpid(), getppid(), getpgrp());

        close(fd[READ]);
        write(fd[WRITE], MSG1, strlen(MSG1) + 1);

        return OK;
    }


    pid_t child_pid2 = fork();

    if (child_pid2 == -1)
    {
        perror("Can't fork child2.");
        return ERROR;
    }
    else if (child_pid2 == 0)
    {
        printf("Child 2: PID = %d, PPID = %d, GPID = %d\n", getpid(), getppid(), getpgrp());

        close(fd[READ]);
        write(fd[WRITE], MSG2, strlen(MSG2) + 1);

        return OK;
    }


    if (child_pid1 && child_pid2)
    {
        char msg1[LEN];
        char msg2[LEN];

        close(fd[WRITE]);

        read(fd[READ], msg1, LEN);
        read(fd[READ], msg2, LEN);

        printf("\nChild 1 wrote : %s\n", msg1);
        printf("Child 2 wrote : %s\n", msg2);

        int status;
        pid_t child_pid = wait(&status);
        printf("\nChild 1 has fihished: child PID = %d, status = %d\n", child_pid, status);
        check_status(status);

        child_pid = wait(&status);
        printf("\nChild 2 has fihished: child PID = %d, status = %d\n", child_pid, status);
        check_status(status);
    }

    return OK;
}
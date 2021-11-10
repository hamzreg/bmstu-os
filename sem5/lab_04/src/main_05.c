#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define OK    0
#define ERROR 1
#define PAUSE 20

#define READ  0
#define WRITE 1

#define MSG1  "HELLO, "
#define MSG2  "WORLD!"
#define LEN   20

#define CTRL_Z 0
#define CTRL_C 1
#define NO     2
#define SIGNAL_MSG "\nPress: \
                    \n-CTRL + Z - for msg from child 1; \
                    \n-CTRL + C - for msg from child 2.\n"

int flag = NO;

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

void catch_ctrlz(int signal)
{
    flag = CTRL_Z;
    printf("\nCatched signal = %d\n", signal);
}

void catch_ctrlc(int signal)
{
    flag = CTRL_C;
    printf("\nCatched signal = %d\n", signal);
}

int main(void)
{
    signal(SIGTSTP, catch_ctrlz);
    signal(SIGINT, catch_ctrlc);

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

        printf(SIGNAL_MSG);
        sleep(PAUSE);

        if (flag == CTRL_Z)
        {
            printf("\nChild 1 wrote : %s\n", msg1);
        }
        else if (flag == CTRL_C)
        {
            printf("\nChild 2 wrote : %s\n", msg2);
        }
        else
        {
            printf("\nNothing is pressed\n");
        }

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
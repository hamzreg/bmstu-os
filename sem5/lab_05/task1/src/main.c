#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "buffer.h"
#include "runners.h"


int main(void)
{
    setbuf(stdout, NULL);

    int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd = shmget(IPC_PRIVATE, sizeof(buffer_t), IPC_CREAT | perms);

    if (fd == -1)
    {
        perror("\nОшибка при создании нового разделяемого сегмента памяти.\n");

        return 1;
    }

    buffer_t *buffer = (buffer_t*)(shmat(fd, 0, 0));

    if (buffer == (void*)-1)
    {
        perror("\nОшибка при получении указателя на сегмент.\n");

        return 2;
    }

    if (init_buffer(buffer) == -1)
    {
        perror("\nОшибка при инициализации буфера.\n");

        return 3;
    }


    int sem_descr = semget(IPC_PRIVATE, 3, IPC_CREAT | perms);

    if (sem_descr == -1)
    {
        perror("\nОшибка при создании набора семафоров.\n");

        return 4;
    }

    if (semctl(sem_descr, 0, SETVAL, 1) == -1)
    {
        perror("\nОшибка при установке бинарного семафора.\n");

        return 5;
    }

    if (semctl(sem_descr, 1, SETVAL, N) == -1)
    {
        perror("\nОшибка при установке семафора 'БуфферПуст'.\n");

        return 6;
    }

    if (semctl(sem_descr, 2, SETVAL, 0) == -1)
    {
        perror("\nОшибка при установке семафора 'БуфферПолон'.\n");

        return 7;
    }



    int child_pid;

    for (size_t i = 0; i < 3; i++)
    {
        if ((child_pid = fork()) == -1)
        {
            perror("\nОшибка при создании процесса производителя.\n");

            return 8;
        }
        else if (child_pid == 0)
        {
            run_producer(buffer, sem_descr, i);
            exit(0);
        }
    }

    for (size_t i = 0; i < 3; i++)
    {
        if ((child_pid = fork()) == -1)
        {
            perror("\nОшибка при создании процесса потребителя.\n");

            return 9;
        }
        else if (child_pid == 0)
        {
            run_consumer(buffer, sem_descr, i);
            exit(0);
        }
    }

    for (size_t i = 0; i < 6; i++)
    {
        int status;

        if (wait(&status) == -1)
        {
            perror("\nОшибка при системном вызове wait().\n");

            return 10;
        }

        if (!WIFEXITED(status))
        {
            printf("\nПроцесс завершился ненормально.\n");
        }
    }


    if (shmdt((void*)buffer) == -1)
    {
        perror("\nОшибка при отключении разделяемого сегмента памяти от адресного пространства.\n");

        return 11;
    }

    if (shmctl(fd, IPC_RMID, NULL) == -1)
    {
        perror("\nОшибка при отметке разделяемого сегмента как удаленного.\n");

        return 12;
    }

    if (semctl(sem_descr, 0, IPC_RMID, 0) == -1)
    {
        perror("\nОшибка при удалении набора семафоров.\n");

        return 13;
    }

    return 0;
} 

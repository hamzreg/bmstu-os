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

#include "constants.h"
#include "runners.h"

int main(void)
{
    setbuf(stdout, NULL);
    
    int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | perms);

    if (fd == -1)
    {
        perror("\nОшибка при создании нового разделяемого сегмента памяти.\n");

        return 1;
    }

    int *shared_counter = (int*)(shmat(fd, 0, 0));

    if (shared_counter == (void*)-1)
    {
        perror("\nОшибка при получении указателя на сегмент.\n");

        return 2;
    }


    int sem_descr = semget(IPC_PRIVATE, 4, IPC_CREAT | perms);

    if (sem_descr == -1)
    {
        perror("\nОшибка при создании набора семафоров.\n");

        return 4;
    }

    if (semctl(sem_descr, ACTIVE_READERS, SETVAL, 0) == -1)
    {
        perror("\nОшибка при установке семафора.\n");

        return 5;
    }

    if (semctl(sem_descr, ACTIVE_WRITERS, SETVAL, 0) == -1)
    {
        perror("\nОшибка при установке семафора.\n");

        return 6;
    }

    if (semctl(sem_descr, WRITERS_QUEUE, SETVAL, 0) == -1)
    {
        perror("\nОшибка при установке семафора.\n");

        return 7;
    }

    if (semctl(sem_descr, READERS_QUEUE, SETVAL, 0) == -1)
    {
        perror("\nОшибка при установке семафора.\n");

        return 8;
    }


    int child_pid;

    for (size_t i = 0; i < 5; i++)
    {
        if ((child_pid = fork()) == -1)
        {
            perror("\nОшибка при создании процесса читателя.\n");

            return 9;
        }
        else if (child_pid == 0)
        {
            run_reader(shared_counter, sem_descr, i);
            exit(0);
        }
    }

    for (size_t i = 0; i < 3; i++)
    {
        if ((child_pid = fork()) == -1)
        {
            perror("\nОшибка при создании процесса писателя.\n");

            return 10;
        }
        else if (child_pid == 0)
        {
            run_writer(shared_counter, sem_descr, i);
            exit(0);
        }
    }

    for (size_t i = 0; i < 8; i++)
    {
        int status;

        if (wait(&status) == -1)
        {
            perror("\nОшибка при системном вызове wait().\n");
            return 11;
        }

        if (!WIFEXITED(status))
        {
            printf("\nПроцесс завершился ненормально.\n");
        }
    }


    if (shmdt((void*)shared_counter) == -1)
    {
        perror("\nОшибка при отключении разделяемого сегмента памяти от адресного пространства.\n");

        return 12;
    }
    if (shmctl(fd, IPC_RMID, NULL) == -1)
    {
        perror("\nОшибка при отметке разделяемого сегмента как удаленного.\n");

        return 13;
    }
    if (semctl(sem_descr, IPC_RMID, 0) == -1)
    {
        perror("\nОшибка при удалении семафора.\n");

        return 14;
    }

    return 0;
} 

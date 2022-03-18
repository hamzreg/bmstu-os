#include "runners.h"

struct sembuf start_read[] = 
{
    {READERS_QUEUE, 1, 0},
    {ACTIVE_WRITERS, 0, 0},
    {WRITERS_QUEUE, 0, 0},
    {ACTIVE_READERS, 1, 0},
    {READERS_QUEUE, -1, 0}
};

struct sembuf stop_read[] =
{
    {ACTIVE_READERS, -1, 0}
};

struct sembuf start_write[] = 
{
    {WRITERS_QUEUE, 1, 0},
    {ACTIVE_READERS, 0, 0},
    {ACTIVE_WRITERS, 0, 0},
    {ACTIVE_WRITERS, 1, 0},
    {WRITERS_QUEUE, -1, 0}
};

struct sembuf stop_write[] =
{
    {ACTIVE_WRITERS, -1, 0}
};

int run_reader(int *const shared_counter, const int sid, const int reader_id)
{
    if (!shared_counter)
    {
        return -1;
    }

    srand(time(NULL) + reader_id);

    int sleep_time;

    for (int i = 0; i < ITERS; i++)
    {
        sleep_time = rand() % TIME_RRANGE + TIME_FROM;
        sleep(sleep_time);

        if (semop(sid, start_read, 5) == -1)
        {
            perror("\n\nЧитатель не может изменить значение семафора.\n\n");
            exit(-1);
        }

        int value = *shared_counter;
        printf("\e[1;32mЧитатель %d прочитал: %5d\e[0m (состояние сна %d с.)\n", reader_id + 1, value, sleep_time);

        if (semop(sid, stop_read, 1) == -1)
        {
            perror("\n\nЧитатель не может изменить значение семафора.\n\n");
            exit(-2);
        }
    }

    return 0;
}

int run_writer(int *const shared_counter, const int sid, const int writer_id)
{
    if (!shared_counter)
    {
        return -1;
    }

    srand(time(NULL) + writer_id + 5);

    int sleep_time;

    for (int i = 0; i < ITERS; i++)
    {
        sleep_time = rand() % TIME_WRANGE + TIME_FROM;
        sleep(sleep_time);

        if (semop(sid, start_write, 5) == -1)
        {
            perror("\n\nПисатель не может изменить значение семафора\n\n");
            exit(-3);
        }

        int value = ++(*shared_counter);
        printf("\e[1;33mПисатель %d записал:  %5d\e[0m (состояние сна %d с.)\n", writer_id + 1, value, sleep_time);

        if (semop(sid, stop_write, 1) == -1)
        {
            perror("\n\nПисатель не может изменить значение семафора.\n\n");
            exit(-4);
        }
    }

    return 0;
}

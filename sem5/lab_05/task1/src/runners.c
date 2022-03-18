#include "runners.h"
#include "buffer.h"
#include "constants.h"

struct sembuf producer_lock[2]    = {{EMPTY_BUF, -1, 0}, {BINARY_SEM, -1, 0}};
struct sembuf producer_release[2] = {{BINARY_SEM, 1, 0}, {FULL_BUF, 1, 0}};

struct sembuf consumer_lock[2] = {{FULL_BUF, -1, 0}, {BINARY_SEM, -1, 0}};
struct sembuf consumer_release[2] = {{BINARY_SEM, 1, 0}, {EMPTY_BUF, 1, 0}};

int run_producer(buffer_t *const buffer, const int sid, const int producer_id)
{
    if (!buffer)
    {
        return -1;
    }

    srand(time(NULL) + producer_id);

    int sleep_time;
    char elem;


    for (int i = 0; i < ITERS; i++)
    {
        sleep_time = rand() % PTIME_RANGE + PTIME_FROM;
        sleep(sleep_time);

        if (semop(sid, producer_lock, 2) == -1)
        {
            perror("\n\nПроизводитель не может изменить значение семафора.\n\n");
            exit(-1);
        }

        elem = buffer->element++;

        if (write_buffer(buffer, elem) == -1)
        {
            perror("\n\nОшибка при записи в буфер.\n\n");
            return -2;
        }

        printf("\e[1;32mПроизводитель %d записал: %4c\e[0m (состояние сна %d с.)\n", producer_id + 1, elem, sleep_time);

        if (semop(sid, producer_release, 2) == -1)
        {
            perror("\n\nПроизводитель не может изменить значение семафора.\n\n");
            exit(-2);
        }
    }

    return 0;
}

int run_consumer(buffer_t *const buffer, const int sid, const int consumer_id)
{
    if (!buffer)
    {
        return -1;
    }

    srand(time(NULL) + consumer_id + 3);

    int sleep_time;
    char elem;

    for (int i = 0; i < ITERS; i++)
    {
        sleep_time = rand() % CTIME_RANGE + CTIME_FROM;
        sleep(sleep_time);

        if (semop(sid, consumer_lock, 2) == -1)
        {
            perror("\n\nПотребитель не может изменить значение семафора.\n\n");
            exit(-3);
        }

        if (read_buffer(buffer, &elem) == -1)
        {
            perror("\n\nОшибка при чтении из буфера.\n\n");

            return -3;
        }

        printf("\e[1;33mПотребитель %d прочитал:  %4c\e[0m (состояние сна %d с.)\n", consumer_id + 1, elem, sleep_time);

        if (semop(sid, consumer_release, 2) == -1)
        {
            perror("\n\nПотребитель не может изменить значение семафора.\n\n");
            exit(-4);
        }
    }

    return 0;
}

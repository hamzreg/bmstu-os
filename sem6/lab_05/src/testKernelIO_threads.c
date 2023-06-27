#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"

typedef struct args
{
    int fd;
    char *color;
} arg_struct;

pthread_mutex_t mutex;

void *print_letter(void *arg) 
{
    pthread_mutex_lock(&mutex);
    arg_struct *now_arg = (arg_struct *) arg;
    int fd = now_arg->fd;

    int flag = 1;
    char c;

    while (flag == 1) 
    {
        flag = read(fd, &c, 1);

        if (flag == 1)
        {
            printf("%s%c" RESET, now_arg->color, c);
        }
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void)
{
    int fd1 = open("alphabet.txt", O_RDONLY);
    arg_struct arg1 = {.fd = fd1, .color = RED};

    int fd2 = open("alphabet.txt", O_RDONLY);
    arg_struct arg2 = {.fd = fd2, .color = BLUE};
    pthread_t tid;
    pthread_create(&tid, NULL, print_letter, &arg2);

    print_letter(&arg1);

    pthread_join(tid, NULL);

    printf("\n");
    return 0;
}

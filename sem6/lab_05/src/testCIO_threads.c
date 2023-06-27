#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"

typedef struct args
{
    FILE *fs;
    char *color;
} arg_struct;

void *print_letter(void *arg) 
{
    arg_struct *now_arg = (arg_struct *) arg;
    FILE *fs = now_arg->fs;

    int flag = 1;
    char c;

    while (flag == 1) 
    {
        flag = fscanf(fs, "%c", &c);

        if (flag == 1)
        {
            fprintf(stdout, "%s%c" RESET, now_arg->color, c);

            sleep(1);
        }
    }

    return NULL;
}

int main(void)
{
    int fd = open("alphabet.txt", O_RDONLY);
    
    FILE *fs1 = fdopen(fd, "r");
    char buff1[20];
    setvbuf(fs1, buff1, _IOFBF, 20);
    arg_struct arg1 = {.fs = fs1, .color = RED};
    
    FILE *fs2 = fdopen(fd, "r");
    char buff2[20];
    setvbuf(fs2, buff2, _IOFBF, 20);
    arg_struct arg2 = {.fs = fs2, .color = BLUE};
    pthread_t tid;
    pthread_create(&tid, NULL, print_letter, &arg2);

    print_letter(&arg1);

    pthread_join(tid, NULL);
    fprintf(stdout, "\n");

    return 0;
}

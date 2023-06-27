#include <stdio.h>
#include <pthread.h>

void *print_letter(void *arg) 
{
    FILE* fd = fopen("results.txt", "a");
    char *c = (char *) arg;

    while (*c <= 'z')
    {
        fprintf(fd, "%c", *c);
        (*c) += 2;
    }

    fclose(fd);

    return NULL;
}

int main(void)
{
    pthread_t tid;
    char c2 = 'b', c1 = 'a';
    pthread_create(&tid, NULL, print_letter, &c2);

    print_letter(&c1);

    pthread_join(tid, NULL);

    return 0;
}

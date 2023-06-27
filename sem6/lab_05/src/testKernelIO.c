#include <fcntl.h>
#include <unistd.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"

int main(void)
{
    char c;    

    int fd1 = open("alphabet.txt", O_RDONLY);
    int fd2 = open("alphabet.txt", O_RDONLY);

    int flag1 = 1, flag2 = 2;

    while (flag1 == 1 || flag2 == 1)
    {
        flag1 = read(fd1, &c, 1);

        if (flag1 == 1)
        {
            printf(RED "%c" RESET, c);
        }

        flag2 = read(fd2, &c, 1);

        if (flag2 == 1)
        {
            printf(BLUE "%c" RESET, c);
        }
    }

    printf("\n");

    return 0;
}

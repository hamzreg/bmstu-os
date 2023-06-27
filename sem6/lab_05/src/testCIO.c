#include <stdio.h>
#include <fcntl.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"

int main(void)
{
    int fd = open("alphabet.txt", O_RDONLY);
    
    FILE *fs1 = fdopen(fd, "r");
    char buff1[20];
    setvbuf(fs1, buff1, _IOFBF, 20);
    
    FILE *fs2 = fdopen(fd, "r");
    char buff2[20];
    setvbuf(fs2, buff2, _IOFBF, 20);
    
    int flag1 = 1, flag2 = 2;
    
    while (flag1 == 1 || flag2 == 1)
    {
        char c;
        flag1 = fscanf(fs1, "%c", &c);

        if (flag1 == 1) 
        {
            fprintf(stdout, RED "%c" RESET, c);
        }

        flag2 = fscanf(fs2, "%c", &c);

        if (flag2 == 1)
        { 
            fprintf(stdout, BLUE "%c" RESET, c); 
        }
    }

    fprintf(stdout, "\n");
    return 0;
}

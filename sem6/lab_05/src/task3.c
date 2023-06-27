#include <stdio.h>
#include <sys/stat.h>

void print_file_info(FILE *fs) 
{
    struct stat buff;
    stat("results.txt", &buff);
    printf("inode: %ld\n", buff.st_ino);
    printf("Размер в байтах: %ld\n", buff.st_size);
    printf("Текущая позиция: %ld\n\n", ftell(fs));
}

int main(void)
{
    char c;    

    FILE* fs1 = fopen("results.txt", "w");
    print_file_info(fs1);

    FILE* fs2 = fopen("results.txt", "w");
    print_file_info(fs2);

    for (char c = 'a'; c <= 'z'; c++) 
    {
        if (c % 2) 
        {
            fprintf(fs1, "%c", c);
        } 
        else 
        {
            fprintf(fs2, "%c", c);
        }
    }

    print_file_info(fs1);
    fclose(fs1);
    print_file_info(fs1);

    print_file_info(fs2);    
    fclose(fs2);
    print_file_info(fs2); 

    return 0;
}

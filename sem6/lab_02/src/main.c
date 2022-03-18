#include <stdio.h>

#include "../inc/stack.h"
#include "../inc/myftw.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("\nНеобходимо передать название каталога.\n");
    }

    my_ftw(argv[1]);

    return 0;
}
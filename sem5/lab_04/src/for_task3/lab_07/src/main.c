#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/array_t.h"
#include "../inc/array.h"
#include "../inc/numbers.h"
#include "../inc/errors.h"
#include "../inc/macrologger.h"

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        LOG_ERROR("%s", "Invalid number of arguments");
        return ERROR_ARGC;
    }


    FILE *in_file = fopen(argv[1], "r");

    if (in_file == NULL)
    {
        LOG_ERROR("%s", "File open error");
        return FILE_OPEN_ERROR;
    }
    LOG_INFO("%s", "Input file open");


    array_t array;
    int code = get_array_len(in_file, &array);
    
    if (code != OK)
        return code;
    LOG_INFO("Array len is %d", array.len);

    array.start = malloc(array.len * sizeof(int));
         
    if (array.start == NULL)
    {
        LOG_ERROR("%s", "Memory allocation error");
        return MEMORY_ERROR;
    }
    LOG_INFO("%s", "Array created");

    rewind(in_file);
    code = fill_array(in_file, array.start, array.start + array.len);

    if (code != OK)
    {
        free(array.start);
        return code;
    }
    LOG_INFO("%s", "Array filled");


    if (fclose(in_file))
    {
        free(array.start);

        LOG_ERROR("%s", "File close error");
        return FILE_CLOSE_ERROR;
    }
    LOG_INFO("%s", "Input file close");


    if (argc == 2)
    {
        printf("Before sort: ");
        print_array(array.start, array.start + array.len);
        mysort(array.start, array.len, sizeof(int), compare_int);
        LOG_INFO("%s", "Array is sorted");

        printf("After sort: ");
        print_array(array.start, array.start + array.len);
        free(array.start);
    }


    if (argc == 3)
    {
        if (strcmp("f", argv[2]) != OK)
        {
            free(array.start);
            LOG_ERROR("%s", "Invalid command");
            return ERROR_COMMAND;
        }


        array_t new_array;
        int *end;

        code = key(array.start, array.start + array.len, &new_array.start, &end);
        free(array.start);

        if (code != OK)
            return code;
        LOG_INFO("%s", "Array filtered");

        new_array.len = end - new_array.start;
        LOG_INFO("New array len is %d", new_array.len);

        mysort(new_array.start, new_array.len, sizeof(int), compare_int);
        LOG_INFO("%s", "Array is sorted");
        
        print_array(new_array.start, new_array.start + new_array.len);
        free(new_array.start);
    }
    LOG_INFO("%s", "Numbers written to file");
   
    return OK;
}

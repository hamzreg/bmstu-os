#include <stdio.h>
#include <stdlib.h>
#include "../inc/array.h"


int get_array_len(FILE *in_file, array_t *const array)
{
    if (array == NULL)
    {
        LOG_ERROR("%s", "Invalid pointer");
        return ERROR_POINTER;
    }

    int array_len = 0, number;

    if (fscanf(in_file, "%d", &number) == 1)
    {
        array_len++;

        while (fscanf(in_file, "%d", &number) == 1)
            array_len++;

        array->len = array_len;

        return OK;
    }

    LOG_ERROR("%s", "Data read error");
    return DATA_ERROR;
}


int fill_array(FILE *in_file, int *const first, int *const last)
{
    if (first == NULL || last == NULL)
    {
        LOG_ERROR("%s", "Invalid pointer");
        return ERROR_POINTER;
    }

    if (first == last)
    {
        LOG_ERROR("%s", "Empty array");
        return EMPTY_ARRAY;
    }

    int number;

    for (int *array_pointer = first; array_pointer != last; array_pointer++)
    {
        if (fscanf(in_file, "%d", &number))
            *array_pointer = number;
    }

    return OK;
}


void even_passage(char *start, char *end, size_t size, int (*compar)(const void *, const void *))
{
    for (char *array_pointer = end; array_pointer > start; array_pointer -= size)
        if (compar(array_pointer, array_pointer - size) <= 0)
            swap(size, array_pointer, array_pointer - size);
}


void odd_passage(char *start, char *end, size_t size, int (*compar)(const void *, const void *))
{
    for (char *array_pointer = start; array_pointer < end; array_pointer += size)
        if (compar(array_pointer, array_pointer + size) >= 0)
            swap(size, array_pointer, array_pointer + size);
}


void mysort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
    if (base == NULL || nmemb <= 0)
        return;

    char *start = base;
    char *end = start + size * (nmemb - 1);

    for (size_t i = 0; i < nmemb; i++)
    {
        if (!(i % 2))
            even_passage(start, end, size, compar);
        else
            odd_passage(start, end, size, compar);
    }
}


int key(const int *pb_src, const int *pe_src, int **pb_dst, int **pe_dst)
{
    if (pb_src == NULL || pe_src == NULL)
    {
        LOG_ERROR("%s", "Invalid pointer");
        return ERROR_POINTER;
    }

    if (pb_src == pe_src)
    {
        LOG_ERROR("%s", "Empty array");
        return EMPTY_ARRAY;
    }


    int filter_len = 0;

    for (const int *array_pointer = pb_src; array_pointer < pe_src - 1; array_pointer++)
    {
        if (sum_numbers(array_pointer + 1, pe_src) < *array_pointer)
            filter_len++;
    }

    if (!filter_len)
    {
        LOG_ERROR("%s", "There are no such elements");
        return ERROR_FILTER;
    }
    LOG_INFO("Count such elements is %d", filter_len);

    *pb_dst = malloc(filter_len * sizeof(int));

    if (pb_dst == NULL)
    {
        LOG_ERROR("%s", "Memory allocation error");
        return MEMORY_ERROR;
    }
    LOG_INFO("%s", "New array created");

    *pe_dst = *pb_dst + filter_len;

    int *new_array_pointer = *pb_dst;
    for (const int *array_pointer = pb_src; array_pointer < pe_src - 1; array_pointer++)
    {
        if (sum_numbers(array_pointer + 1, pe_src) < *array_pointer)
        {
            *new_array_pointer = *array_pointer;
            new_array_pointer++;
        }
    }
    LOG_INFO("%s", "Numbers written");

    return OK;
}


void write_array_file(FILE *out_file, const int *const first, const int *const second)
{
    for (const int *pa = first; pa != second; pa++)
        fprintf(out_file, "%d ", *pa);
}

void print_array(const int *const first, const int *const second)
{
    for (const int *pa = first; pa != second; pa++)
        printf("%d ", *pa);
    
    printf("\n");
}

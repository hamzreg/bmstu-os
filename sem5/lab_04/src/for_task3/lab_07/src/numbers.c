#include <stdio.h>
#include "../inc/numbers.h"


int compare_int(const void *first, const void *second)
{
    if (first == NULL || second == NULL)
    {
        LOG_ERROR("%s", "Invalid pointer");
        return ERROR_POINTER;
    }

    return *(int *)first - *(int *)second;
}


void swap(size_t size, char *number_one, char *number_two)
{
    char add_number;

    while (size > 0)
    {
        add_number = *number_one;
        *number_one = *number_two;
        *number_two = add_number;

        number_one++;
        number_two++;
        size--;
    }
}


int sum_numbers(const int *first, const int *last)
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

    int sum = 0;

    for (const int *array_pointer = first; array_pointer < last; array_pointer++)
        sum += *array_pointer;

    return sum;
}


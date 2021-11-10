#include "../inc/my_strings.h"

int my_getline(char **lineptr, int *n, FILE *stream)
{
    if (*n == 0)
    {
        *lineptr = malloc(MAGIC_SIZE * sizeof(char));

        if (!(*lineptr))
            return ZERO;

        LOG_INFO("%s", "Memory allocated");
        *n = MAGIC_SIZE;
    }

    int len = 0;
    char symbol;

    while ((symbol = getc(stream)) != '\n')
    {
        LOG_INFO("%d - %c", len, symbol);

        if (len == *n - 1)
        {
            char *pointer = realloc(*lineptr, *n * FACTOR * sizeof(char *));

            if (pointer)
                *lineptr = pointer;
            else
            {
                free(*lineptr);
                return len;
            }
        }

        (*lineptr)[len++] = symbol;
    }

    (*lineptr)[len] = '\0';
    *n = len + 1;

    return *n;
}


int my_strlen(const char *str)
{
    int i = 0;

    while (str[i] != '\0')
        i++;

    return i;
}


char *my_strdup(const char *str)
{
    int len = my_strlen(str);

    char *new_string = malloc((len + 1) * sizeof(char));

    if (!new_string)
        return NULL;

    int i = 0;

    while (str[i] != '\0')
    {
        new_string[i] = str[i];
        i++;
    }

    new_string[i] = '\0';

    return new_string;
}

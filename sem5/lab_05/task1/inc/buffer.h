#ifndef BUFFER_H
#define BUFFER_H

#include <unistd.h>
#include <string.h>

#include "constants.h"

typedef struct
{
    size_t rpos;
    size_t wpos;

    char element;
    char data[N];
} buffer_t;

int init_buffer(buffer_t *const buffer);
int read_buffer(buffer_t *const buffer, char *const elem);
int write_buffer(buffer_t *const buffer, const char elem);

#endif

#ifndef MY_STRINGS_H
#define MY_STRINGS_H

#include <stdio.h>
#include <stdlib.h>
#include "macrologger.h"
#include "constants.h"

int my_getline(char **lineptr, int *n, FILE *stream);

int my_strlen(const char *str);

char *my_strdup(const char *str);

#endif

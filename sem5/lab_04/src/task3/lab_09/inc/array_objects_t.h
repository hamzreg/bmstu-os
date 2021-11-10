#ifndef ARRAY_OBJECTS_T
#define ARRAY_OBJECTS_T

#include "object_t.h"

typedef struct
{
    object_t *array;
    int count_object;
} array_objects_t;

#endif

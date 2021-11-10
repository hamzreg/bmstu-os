#ifndef ARRAY_OBJECTS_H
#define ARRAY_OBJECTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array_objects_t.h"
#include "macrologger.h"
#include "return_codes.h"
#include "constants.h"
#include "strings.h"
#include "object.h"

int get_array_len(FILE *f_in, array_objects_t *const objects);

void sort_array_objects(object_t *const objects, const int end);

void print_array_objects(const object_t *const array_objects, const int count);

void print_some_objects(const char *const substring, const object_t *const array_objects, const int count);

void free_array_objects(array_objects_t *objects);

int create_array_objects(FILE *f_in, array_objects_t *const objects);

#endif

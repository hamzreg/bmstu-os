#ifndef BASE_FUNCTIONS_H
#define BASE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array_objects_t.h"
#include "array_objects.h"
#include "return_codes.h"
#include "constants.h"

int get_objects(const char *const file_name, array_objects_t *const objects);

void get_sorted_objects(array_objects_t *const objects);

void get_some_objects(const char *substring, array_objects_t *const objects);

#endif

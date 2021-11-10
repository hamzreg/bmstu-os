#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>

#include "object_t.h"
#include "my_strings.h"
#include "macrologger.h"
#include "return_codes.h"

int read_object(FILE *f_in, object_t *const object);

void change_objects(object_t *first_object, object_t *second_object);

int copy_objects(object_t *first_object, const object_t second_object);

void print_array_object(const object_t object);

#endif

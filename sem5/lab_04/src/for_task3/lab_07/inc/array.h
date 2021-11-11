#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "array_t.h"
#include "numbers.h"
#include "errors.h"
#include "macrologger.h"


int get_array_len(FILE *in_file, array_t *const array);

int fill_array(FILE *in_file, int *const first, int *const last);

void mysort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

int key(const int *pb_src, const int *pe_src, int **pb_dst, int **pe_dst);

void write_array_file(FILE *out_file, const int *const first, const int *const second);

void print_array(const int *const first, const int *const second);

#endif

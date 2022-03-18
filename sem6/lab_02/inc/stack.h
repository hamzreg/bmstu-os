#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include "item.h"

typedef struct  stack
{
    int count;
    item_t *head;
} stack_t;

void init_stack(stack_t *tree);

void free_stack(item_t *tree);

void push_stack(stack_t *tree, char *name, const int level);

item_t* pop_stack(stack_t *tree);

int is_empty_stack(stack_t *tree);

#endif

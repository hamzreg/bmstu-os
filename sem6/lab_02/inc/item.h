#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct item
{
    char *name;
    int level;
    struct item *next;
} item_t;

item_t* create_item(char *name, const int level);

void free_item(item_t *item);

#endif

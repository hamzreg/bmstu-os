#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "node.h"

node_t *reverse(node_t *head);

void sorted_insert(node_t **head, node_t *element,
int (*comparator)(const void *, const void *));

node_t *sort(node_t *head, int (*comparator)(const void *, const void *));

int compare_int(const void *first, const void *second);

#endif

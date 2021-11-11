#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>

#include "node_t.h"
#include "macrologger.h"

node_t *create_node(void *data);

void add_node(node_t **head, node_t *node);

void *pop_front(node_t **head);

void *pop_back(node_t **head);

#endif

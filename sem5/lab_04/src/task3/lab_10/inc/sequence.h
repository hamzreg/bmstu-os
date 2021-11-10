#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "return_codes.h"
#include "constants.h"
#include "macrologger.h"
#include "sequence_t.h"
#include "node_t.h"
#include "node.h"

void init_sequence(sequence_t *sequence);

int read_sequence(const char *filename, sequence_t *sequence,
int **storage);

void create_sequence(sequence_t *sequence, int *storage);

void free_sequence(node_t *head);

int is_palindrome(sequence_t *sequence);

node_t *process_sequence(const char *filename, sequence_t *sequence,
int *storage, int (*comparator)(const void *, const void *));

int write_sequence(const char *filename, sequence_t *sequence);

#endif

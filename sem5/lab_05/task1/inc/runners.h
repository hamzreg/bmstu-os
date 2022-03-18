#ifndef RUNNERS_H
#define RUNNERS_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/sem.h>

#include "buffer.h"

int run_producer(buffer_t *const buffer, const int sid, const int prod_id);
int run_consumer(buffer_t *const buffer, const int sid, const int cons_id);

#endif

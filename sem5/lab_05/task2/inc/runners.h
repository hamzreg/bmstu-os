#ifndef RUNNERS_H
#define RUNNERS_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/sem.h>

#include "constants.h"

int run_reader(int *const shared_counter, const int sid, const int read_id);
int run_writer(int *const shared_counter, const int sid, const int write_id);

#endif

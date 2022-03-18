#include <dirent.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "stack.h"

void my_ftw(char *pathname);

//int print_tree(const char *const filename, int offset);

//int process_item(char *now_path, char *now_file, stack_t* tree, const int level);
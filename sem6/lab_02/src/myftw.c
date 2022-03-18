#include "../inc/myftw.h"

char* get_fullpath(char *pathname)
{
    char *fullpath = malloc(PATH_MAX + 1);

    if (!fullpath)
    {
        printf("\nОшибка! Не удалось получить значение.\n");
        return 1;
    }

    strcpy(fullpath, pathname);

    return fullpath;
}

int print_tree(const char *const filename, int offset)
{
    printf(" ");

    for (int i = 0; i < offset; i++)
    {
        if (i == offset - 1)
        {
            printf("\033[1;33m ├───\033[0m");
        }
        else
        {
            printf("\033[1;33m │   ");
        }
    }

    printf("\033[1;32m %s\033[0m\n", filename);

    return 0;
}

char *join(char *path, char *new)
{
    int n = strlen(path);

    char *new_path = malloc(n + strlen(new) + 2);

    if (new_path == NULL)
    {
        printf("Ошибка! Не удалось создать новый путь.\n");
        return 1;
    }
    else
    {
        strcpy(new_path, path);
        new_path[n++] = '/';
        strcpy(new_path + n, new);

        return new_path;
    }
}

int process_item(char *now_path, char *now_file, stack_t* tree, const int level)
{
    struct stat statbuf;

    char *fullpath = join(now_path, now_file);

    if (lstat(fullpath, &statbuf) < 0)
    {
        free(fullpath);

        return -1;
    }
    else if (S_ISDIR(statbuf.st_mode) != 0)
    {
        push_stack(tree, fullpath, level);
        return 0;
    }
    else
    {
        free(fullpath);

        return print_tree(now_file, level);
    }
}

void my_ftw(char *pathname)
{
    struct dirent* dirp;
    DIR* dp;

    stack_t tree;
    init_stack(&tree);

    char *root_path = get_fullpath(pathname);
    item_t *item;
    int level = 0;
    push_stack(&tree, root_path, level);

    while (!is_empty_stack(&tree))
    {
        item = pop_stack(&tree);

        char *file;

        if ((file = strrchr(item->name, '/')))
        {
            print_tree(file + 1, item->level);
        }
        else
        {
            print_tree(item->name, item->level);
        }

        if ((dp = opendir(item->name)) != NULL)
        {
            while ((dirp = readdir(dp)) != NULL)
            {
                if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0)
                {
                    process_item(item->name, dirp->d_name, &tree, item->level + 1);
                }
            }
        }

        free_item(item);

        if (closedir(dp) < 0)
        {
            printf("Ошибка! Не удалось закрыть директорию.\n");
        }
    }

    free_stack(tree.head);
}

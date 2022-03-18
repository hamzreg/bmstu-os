#include "../inc/item.h"

item_t* create_item(char *name, const int level)
{
    item_t *item = malloc(sizeof(item_t));

    if (item)
    {
        item->name = name;
        item->level = level;
        item->next = NULL;
    }

    return item;
}

void free_item(item_t *item)
{
    free(item->name);
    free(item);
}

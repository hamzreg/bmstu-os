#include "../inc/stack.h"

void init_stack(stack_t *tree)
{
    tree->count = 0;
    tree->head = NULL;
}

void free_stack(item_t *tree)
{
    item_t *item;

    for (; tree; tree = item)
    {
        item = tree->next;
        free_item(tree);
    }
}

void push_stack(stack_t *tree, char *name, const int level)
{
    item_t *item = create_item(name, level);

    if (item)
    {
        tree->count = tree->count + 1;
        item->next = tree->head;
        tree->head = item;
    }
}

item_t* pop_stack(stack_t *tree)
{
    if (!tree->head)
    {
        printf("\nОшибка! Стек пуст.\n");
        exit(1);
    }

    item_t *add = tree->head;
    tree->head = tree->head->next;
    tree->count -= 1;

    return add;
}

int is_empty_stack(stack_t *tree)
{
    return !tree->count;
}

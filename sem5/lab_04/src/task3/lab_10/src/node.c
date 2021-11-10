#include "../inc/node.h"

node_t *create_node(void *data)
{
    node_t *node = malloc(sizeof(node_t));

    if (node)
    {
        node->data = data;
        node->next = NULL;
    }

    return node;
}


void add_node(node_t **head, node_t *node)
{
    node->next = *head;
    *head = node;
}


void *pop_front(node_t **head)
{
    if (!head || !(*head))
        return NULL;

    node_t *node = (*head);
    void *data = node->data;
    (*head) = (*head)->next;

    free(node);

    return data;
}


void *pop_back(node_t **head)
{
    if (!head || !(*head))
        return NULL;

    node_t *node = *head, *prev_node = NULL;

    for (; node->next; node = node->next)
        prev_node = node;

    void *data = node->data;

    if (prev_node)
        prev_node->next = node->next;
    else
        *head = node->next;

    free(node);

    return data;
}

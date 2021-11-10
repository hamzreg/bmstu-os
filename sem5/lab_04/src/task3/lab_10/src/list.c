#include "../inc/list.h"

node_t *reverse(node_t *head)
{
    if (!head)
        return NULL;

    node_t *result = NULL;
    node_t *node;

    while (head != NULL)
    {
        void *element = pop_front(&head);
        node = create_node(element);
        add_node(&result, node);
    }

    return result;
}


void sorted_insert(node_t **head, node_t *element,
int (*comparator)(const void *, const void *))
{
    element->next = NULL;

    if (!(*head))
    {
        element->next = *head;
        *head = element;

        return;
    }

    char *first, *second = element->data;

    if ((*head)->next == NULL)
    {
        first = (*head)->data;

        if (comparator(first, second) <= 0)
        {
            (*head)->next = element;
            element->next = NULL;

            return;
        }

        element->next = *head;
        *head = element;

        return;
    }

    node_t *node = *head, *add;
    first = node->data;

    if (comparator(first, second) > 0)
    {
        add = *head;
        *head = element;
        element->next = add;

        return;
    }

    while (node->next)
    {
        first = node->next->data;

        if (comparator(first, second) > 0)
        {
            add = node->next;
            node->next = element;
            element->next = add;

            return;
        }

        node = node->next;
    }

    node->next = element;
}


node_t *sort(node_t *head, int (*comparator)(const void *, const void *))
{
    if (!head)
        return NULL;

    if (!comparator)
        return NULL;

    node_t *result = NULL;

    while (head != NULL)
    {
        node_t *node = head;
        head = head->next;
        sorted_insert(&result, node, comparator);
    }

    return result;
}


int compare_int(const void *first, const void *second)
{
    return *(int *)first - *(int *)second;
}

#include "stack.h"
#include <stdlib.h>

#define POP_ERROR -1

node_t *push(const node_t *const head, const ntype value)
{
    node_t *tmp = (node_t *)malloc(sizeof(node_t));
    if (!tmp)
        return NULL;
    tmp->data = value;
    tmp->next = head;
    return tmp;
}

ntype pop(node_t **head)
{
    if (!head)
        return POP_ERROR;

    ntype tmp = (*head)->data;
    node_t *temp = *head;
    *head = (*head)->next;
    free(temp);
    return tmp;
}
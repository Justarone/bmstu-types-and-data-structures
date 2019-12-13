#include "stack.h"
#include <stdlib.h>

node_t *push(const node_t *const head, const ntype value)
{
    node_t *tmp = (node_t *)malloc(sizeof(node_t));
    if (!tmp)
        return NULL;
    tmp->data = value;
    return tmp;
}

ntype pop(node_t *head)
{
    ntype tmp = head->data;
    node_t *temp = head;
    head = head->next;
    free(temp);
    return tmp;
}
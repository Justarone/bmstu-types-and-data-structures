#include "parser.h"
#include <stdlib.h>

#define READED 1
#define PARSE_ERROR 10
#define ADD_ERROR 11
#define OK 0

static int add(node_t **array, const int pos, const int value)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node)
        return ADD_ERROR;

    new_node->data = value;
    new_node->next = NULL;

    // если это первый элемент в списке смежности
    if (!array[pos])
    {
        array[pos] = new_node;
        return OK;
    }

    node_t *elem = array[pos];
    for (; elem->next; elem = elem->next)
        ;
    elem->next = new_node;
    return OK;
}

int parser(node_t ***adj_list, int *const size, FILE *const stream)
{
    if (fscanf(stream, "%d", size) != READED)
        return PARSE_ERROR;

    node_t **array = (node_t **)calloc(*size, sizeof(node_t *));
    if (!array)
        return PARSE_ERROR;
    *adj_list = array;

    int pair[2] = {};
    while (fscanf(stream, "%d%d", pair, pair + 1) == READED * 2)
    {
        if (add(array, pair[0], pair[1]))
            return PARSE_ERROR;
        if (add(array, pair[1], pair[0]))
            return PARSE_ERROR;
    }
    return OK;
}

int avoid_cpy(node_t ***target, node_t **source, const int size, const int avnum)
{
    node_t **array = (node_t **)calloc(size, sizeof(node_t *));
    if (!array)
        return PARSE_ERROR;
    *target = array;
    node_t *elem;

    for (int i = 0; i < size; i++)
    {
        elem = source[i];
        if (i == avnum)
            continue;
        while (elem)
        {
            if (elem->data != avnum)
                add(array, i, elem->data);
            elem = elem->next;
        }
    }
}

#include "hash.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

#define OK 0
#define GET_ERROR -2
#define ARG_ERROR -1
#define ADD_ERROR 1
#define REMOVE_ERROR 2
#define ALLOCATION_ERROR 4
#define REBASE_ERROR 5
#define ALREADY_EXISTS 17

#define COLLISION_MAX 4

// this function find the least number which is simple but bigger than num
int next_simple(const int num)
{
    int flag;
    int limit, current = num + 1 + (num % 2);
    while (1)
    {
        flag = 1;
        limit = sqrt(current) + 1;
        for (int i = 2; i <= limit; i++)
            if (current % i == 0)
            {
                flag = 0;
                break;
            }
        if (flag)
            return current;
        current += 2;
    }
}

// initialization of the hash-table (table and array of data also)
// argument is the size of table*
hash_t *init_table(const int base)
{
    hash_t *table = (hash_t *)malloc(sizeof(hash_t));
    if (!table)
        return NULL;

    table->data = (node_h **)calloc(base, sizeof(node_h *));
    if (!table->data)
        return NULL;

    table->base = base;
    return table;
}

// desctruction of hash-table
void destruct_table(hash_t *table)
{
    node_h *tmp;
    for (int i = 0; i < table->base; i++)
        while (table->data[i])
        {
            tmp = table->data[i]->next;
            free(table->data[i]->data);
            table->data[i]->data = NULL;
            free(table->data[i]);
            table->data[i] = tmp;
        }
    free(table->data);
    free(table);
}

// rebase - creates new table with base = new_base and recounts every element's
// hash value
int rebase(hash_t **table, const int new_base)
{
    hash_t *new_table = init_table(new_base);
    if (!new_table)
        return ALLOCATION_ERROR;

    node_h *elem;
    for (int i = 0; i < (*table)->base; i++)
    {
        elem = (*table)->data[i];
        while (elem)
        {
            if (add(new_table, elem->data))
            {
                destruct_table(new_table);
                return REBASE_ERROR;
            }
            elem = elem->next;
        }
    }
    destruct_table(*table);
    *table = new_table;
    return OK;
}

// this function counts hash sum (value)
int hash_function(const char *const key, const int base)
{
    if (!key)
        return ARG_ERROR;
    int sum = 0, i = -1;
    while (key[++i])
        sum += key[i];
    return sum % base;
}

// creates new node with data and writes key in this element (key is the only
// argument)
node_h *create_node_h(const char *const key)
{
    node_h *tmp = (node_h *)calloc(1, sizeof(node_h));
    if (!tmp)
        return NULL;
    tmp->data = (char *)malloc((strlen(key) + 1) * sizeof(char));
    if (!tmp->data)
    {
        free(tmp);
        return NULL;
    }
    strcpy(tmp->data, key);
    return tmp;
}

// adds element in hash table
// if the element make table reach max level of collision
// this function returns error code (not ok)
int add(hash_t *const table, const char *const key)
{
    if (!key || !table)
        return ADD_ERROR;

    int pos = hash_function(key, table->base);

    // if it is first element with this hash
    if (!table->data[pos])
    {
        node_h *tmp = create_node_h(key);
        if (!tmp)
            return ALLOCATION_ERROR;
        table->data[pos] = tmp;
        return OK;
    }

    node_h *elem = table->data[pos];
    for (int i = 0; i < COLLISION_MAX - 1; i++)
    {
        // if key is already exists in table
        if (!strcmp(elem->data, key))
            return ALREADY_EXISTS;
        // add element if this element is the last in this row
        if (!elem->next)
        {
            node_h *tmp = create_node_h(key);
            if (!tmp)
                return ALLOCATION_ERROR;
            elem->next = tmp;
            return OK;
        }
        // get to the next element
        elem = elem->next;
    }
    return ADD_ERROR;
}

// this function checks if the element is in the table or not
int get(hash_t *const table, const char *const key)
{
    if (!key || !table)
        return GET_ERROR;

    int pos = hash_function(key, table->base);
    node_h *elem = table->data[pos];
    while (elem)
    {
        if (!strcmp(elem->data, key))
            return OK;
        elem = elem->next;
    }
    return GET_ERROR;
}

int remove_h(hash_t *const table, const char *const key, stat_t *const stat)
{
    if (!key || !table)
        return REMOVE_ERROR;

    int pos = hash_function(key, table->base);
    node_h *elem = table->data[pos];
    if (!elem)
        return REMOVE_ERROR;

    ++stat->comp_num;
    if (!strcmp(elem->data, key))
    {
        elem = elem->next;
        free(table->data[pos]->data);
        free(table->data[pos]);
        table->data[pos] = elem;
        return OK;
    }
    while (elem->next)
    {
        ++stat->comp_num;
        if (!strcmp(elem->next->data, key))
        {
            node_h *tmp = elem->next->next;
            free(elem->next->data);
            free(elem->next);
            elem->next = tmp;
            return OK;
        }
        elem = elem->next;
    }
    return REMOVE_ERROR;
}

#include "stack_cdio.h"
#include "dynamic_array_utils.h"
#include "stack_utils.h"
#include <stdlib.h>
#include <stdio.h>

#define NOT_IN -1
#define READ_ERROR 15
#define ALLOCATION_ERROR 17
#define PUSH_ERROR 27
#define SIZE_ERROR 37
#define OK 0
#define TOO_BIG_N 10
#define STACK_SIZE 1000
#define MAX_AMOUNT 1000
#define MANUAL_MODE 0

int freed_zone = 0;
int new_zone = 0;
// set values of the flags to zero and
// it will only check as much times as deleted elems
static int flag_a = 1000000;
static int flag_l = 1000000;

// add to classic (list) stack: returns NULL if unseccessful
// else address of new ps
// note for myself: i only use adress of the new memory zone, so i don't need `**`
node_t *push_st_l(node_t *const ps, array_d *const free_zones)
{
    if (ps == NULL)
    {
        node_t *new_ps = (node_t *)malloc(sizeof(node_t));
        new_ps->value = (void *)new_ps;
        new_ps->number = 0;
        new_ps->next = NULL;
        return new_ps;
    }
    if (ps->number >= STACK_SIZE - 1)
        return NULL;
    node_t *new_ps = (node_t *)malloc(sizeof(node_t));
    if (!new_ps)
        return NULL;

    if (flag_l)
    {
        int pos;
        if ((pos = is_in(free_zones, new_ps)) != NOT_IN)
        {
            flag_l--;
            freed_zone++;
            delete_element(free_zones, pos);
        }
        else
            new_zone++;
    }
    new_ps->next = ps;
    new_ps->value = (void *)new_ps;
    new_ps->number = ps->number + 1;
    return new_ps;
}

// add to array-stack: returns 0 if successful else - error code (it
// updates stack in arguments)
int push_st_a(stack_a *const ps, void *const value,
              array_d *const free_zones)
{
    int len = 0;
    if (ps->data)
        len = ps->last - ps->data + 1;
    if (len >= STACK_SIZE)
        return SIZE_ERROR;
    void **temp = (void **)realloc(ps->data, (++len) * sizeof(void *));
    if (!temp)
        return ALLOCATION_ERROR;
    if (ps->data == NULL)
    {
        int pos;
        if ((pos = is_in(free_zones, temp)) != NOT_IN)
        {
            flag_l--;
            freed_zone++;
            delete_element(free_zones, pos);
        }
        else
            new_zone++;
        ps->last = (ps->data = temp) + len - 1;
        *(ps->data) = value;
        return OK;
    }
    ps->last = ps->data + len - 2;
    *(ps->data + len - 1) = value;

    int pos;
    if (ps->data == temp)
    {
        if (flag_a)
        {
            if ((pos = is_in(free_zones, ps->last)) != NOT_IN)
            {
                flag_a--;
                freed_zone++;
                delete_element(free_zones, pos);
            }
            else
                new_zone++;
        }
    }

    else
    {
        for (int i = 0; i < len; i++)
        {
            flag_a++;
            insert_element(free_zones, ps->data + i);
        }

        for (void **i = ps->data; i <= ps->last && flag_a > 0; i++)
        {
            if ((pos = is_in(free_zones, i)) != NOT_IN)
            {
                flag_a--;
                freed_zone++;
                delete_element(free_zones, pos);
            }
            else
                new_zone++;
        }
    }
    ps->last = (ps->data = temp) + len - 1;
    return OK;
}

// pushes n elements list-stack
// p.s.: only works with correct n (n > 0)
node_t *add_st_l(node_t **const ps, int *const n, array_d *const free_zones)
{
    // error: too big n;
    if (*n > MAX_AMOUNT || *n <= 0)
        return NULL;
    node_t *new_node = push_st_l(*ps, free_zones);
    if (!new_node)
    {
        *n = 0;
        return NULL;
    }
    else
    {
        int pos;
        if ((pos = is_in(free_zones, new_node)) != NOT_IN)
        {
            flag_a--;
            freed_zone++;
            delete_element(free_zones, pos);
        }
        else
            new_zone++;
    }

    node_t *temp;
    for (int i = 0; i < *n - 1; i++)
    {
        temp = push_st_l(new_node, free_zones);
        if (!temp)
        {
            *n = i + 1;
            *ps = new_node;
            return NULL;
        }
        else
            new_node = temp;
    }
    return new_node;
}

// pushes n elements in array-stack
int add_st_a(stack_a *const ps, int *const n, array_d *const free_zones,
             const int mode)
{
    long long int buf;
    if (*n > MAX_AMOUNT || *n < 0)
        return TOO_BIG_N;

    void *value;

    if (mode == MANUAL_MODE)
        printf("Введите адрес в виде десятичного или шестнадцатиричного числа"
               "\n(помните, что адрес должен быть в пределах от 0 до 0xFFFFFFFF):\n");
    for (int i = 0; i < *n; i++)
    {
        if (mode == MANUAL_MODE)
            printf("Число №%d: ", i + 1);
        if ((buf = scan_value(mode)) < 0)
        {
            *n = i;
            return READ_ERROR;
        }
        value = (void *)buf;
        if (push_st_a(ps, value, free_zones))
        {
            *n = i;
            return PUSH_ERROR;
        }
    }
    return OK;
}

// pop function for list-stack
// the second argument is array with freed zones (to check fragmentation)
void *pop_l(node_t **ps, array_d *const free_zones)
{
    if (!(*ps))
    {
        return NULL;
    }
    flag_l++;
    node_t *temp = *ps;
    *ps = (*ps)->next;
    void *value = temp->value;

    insert_element(free_zones, temp);
    free(temp);
    return value;
}

// pop function for array-stack
// the second argument is array with freed zones (to check fragmentation)
void *pop_a(stack_a *const array_stack, array_d *const free_zones)
{
    if (!array_stack->data)
        return NULL;

    int len = array_stack->last - array_stack->data + 1;
    if (len <= 0)
        return NULL;

    void *value = *array_stack->last;
    if (len == 1)
    {
        flag_a++;
        insert_element(free_zones, array_stack->data);
        free(array_stack->data);
        array_stack->data = NULL;
        return value;
    }

    void **temp = (void **)realloc(array_stack->data, (--len) * sizeof(void *));
    if (!temp)
        return NULL;
    if (array_stack->data == temp)
    {
        insert_element(free_zones, array_stack->last);
        flag_a++;
    }
    else
    {
        for (int i = 0; i < len; i++)
        {
            flag_a++;
            insert_element(free_zones, array_stack->data + i);
        }

        array_stack->data = temp;
        int pos;
        for (void **i = array_stack->data; i <= array_stack->data + len - 1 && flag_a > 0; i++)
        {
            if ((pos = is_in(free_zones, i)) != NOT_IN)
            {
                flag_a--;
                freed_zone++;
                delete_element(free_zones, pos);
            }
            else
                new_zone++;
        }
    }
    array_stack->last = (array_stack->data = temp) + len - 1;
    return value;
}

// cleans n elements of array-stack (if n is bigger than the number of elements
// it cleans all stack)
// returns number of cleaned elements
int cleann_l(node_t **ps, const int n, array_d *const free_zones)
{
    for (int i = 0; i < n; i++)
        if (!pop_l(ps, free_zones))
            return i;
    return n;
}

// cleans n elements of list-stack (if n is bigger than the size of stack
// it cleans all stack)
// returns number of cleaned elements
int cleann_a(stack_a *const array_stack, const int n, array_d *const free_zones)
{
    for (int i = 0; i < n; i++)
        if (!pop_a(array_stack, free_zones))
            return i;
    return n;
}
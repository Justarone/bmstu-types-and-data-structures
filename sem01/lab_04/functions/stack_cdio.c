#include "stack_cdio.h"
#include "stack_utils.h"
#include "dynamic_array_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

// the iteration variable is used for me to check on what size of stack-array
// the realloc (like real realloc, when the begin of array changes its place)
// works
// int iteration = 0;
// add to classic (list) stack: returns NULL if unseccessful
// else address of new ps
// note for myself: i only use adress of the new memory zone, so i don't need `**`
node_t *push_st_l(node_t *const ps)
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
    new_ps->next = ps;
    new_ps->value = (void *)new_ps;
    new_ps->number = ps->number + 1;
    return new_ps;
}

// add to array-stack: returns 0 if successful else - error code (it
// updates stack in arguments)
int push_st_a(stack_a *const ps, void *const value)
{
    if (ps->cur_index >= STACK_SIZE)
        return SIZE_ERROR;
    ps->data[ps->cur_index++] = value;
    return OK;
}

// pushes n elements list-stack
// p.s.: only works with correct n (n > 0)
node_t *add_st_l(node_t **const ps, int *const n, long int *const add_time)
{
    // error: too big n;
    if (*n > MAX_AMOUNT || *n <= 0)
        return NULL;

    *add_time = 0;
    long int start = clock();
    node_t *new_node = push_st_l(*ps);
    if (!new_node)
    {
        *add_time = clock() - start;
        *n = 0;
        return NULL;
    }
    *add_time += clock() - start;

    node_t *temp;
    for (int i = 0; i < *n - 1; i++)
    {
        start = clock();
        temp = push_st_l(new_node);
        if (!temp)
        {
            *n = i + 1;
            *ps = new_node;
            *add_time += clock() - start;
            return NULL;
        }
        new_node = temp;
        *add_time += clock() - start;
    }
    return new_node;
}

// pushes n elements in array-stack
int add_st_a(stack_a *const ps, int *const n, const int mode, long int *const add_time)
{
    *add_time = 0;
    long long int buf;
    if (*n > MAX_AMOUNT || *n < 0)
        return TOO_BIG_N;

    void *value;
    long int start; // variable for start of timer

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
        // the iteration variable shows on which size we are at the moment
        // iteration = ps->last - ps->data + 1;
        // alternative variant is to set iteration variable as the number of
        // iteration of cycle in add_st_a (like `i`)
        start = clock();
        if (push_st_a(ps, value))
        {
            *add_time += clock() - start;
            *n = i;
            return PUSH_ERROR;
        }
        *add_time += clock() - start;
    }
    return OK;
}

// pop function for list-stack
// the second argument is array with freed zones (to check fragmentation)
void *pop_l(node_t **ps)
{
    if (!(*ps))
    {
        return NULL;
    }
    node_t *temp = *ps;
    *ps = (*ps)->next;
    void *value = temp->value;

    free(temp);
    return value;
}

// pop function for array-stack
// the second argument is array with freed zones (to check fragmentation)
void *pop_a(stack_a *const array_stack)
{
    if (array_stack->cur_index <= 0)
        return NULL;
    return array_stack->data[--array_stack->cur_index];
}

// cleans n elements of array-stack (if n is bigger than the number of elements
// it cleans all stack)
// returns number of cleaned elements
int cleann_l(node_t **ps, const int n, long int *const clean_time, array_d *const free_zones)
{
    *clean_time = 0;
    void *temp = NULL;
    long int start;
    for (int i = 0; i < n; i++)
    {
        start = clock();
        if (!(temp = pop_l(ps)))
        {
            *clean_time += clock() - start;
            return i;
        }
        *clean_time += clock() - start;
        insert_element(free_zones, temp);
    }
    return n;
}

// cleans n elements of list-stack (if n is bigger than the size of stack
// it cleans all stack)
// returns number of cleaned elements
int cleann_a(stack_a *const array_stack, const int n, long int *const clean_time)
{
    long int start;
    *clean_time = 0;
    for (int i = 0; i < n; i++)
    {
        start = clock();
        if (!pop_a(array_stack))
        {
            *clean_time += clock() - start;
            return i;
        }
        *clean_time += clock() - start;
    }
    return n;
}
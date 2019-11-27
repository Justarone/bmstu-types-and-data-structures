#include "queue_cdio.h"
#include <stdlib.h>

#define OK 0
#define OVERFLOW_ERROR 1
#define EMPTY_QUEUE 2
#define INSERTION_ERROR 3

// this function receives pointer on the tail of the queue and appends
// one more element (returns new tail if successful else NULL)
node_t *push_l(queue_l *const queue, const int value)
{
    if (queue->size >= QUEUE_SIZE)
        return NULL;

    node_t *tmp = (node_t *)malloc(sizeof(node_t));
    // can't allocate memory for new element in queue
    if (!tmp)
        return NULL;

    tmp->next = NULL;
    tmp->data = value;
    // old tail points on the new one (the new tail is the last in queue)
    if (queue->pin)
        queue->pin->next = tmp;
    else
        queue->pout = tmp;
    queue->size++;
    return tmp;
}

// this function inserts element on the pos position of the list queue (or in the end
// if pos >= queue size)
int ins_l(queue_l *const queue, const double value, const int pos)
{
    if (queue->size >= QUEUE_SIZE)
    {
        // removing last element of the queue to insert the
        // element of the second type
        int val;
        pop_l(queue, &val);
    }

    if (queue->size == 0)
    {
        node_t *res = push_l(queue, value);
        if (!res)
            return INSERTION_ERROR;
        queue->pin = res;
        return OK;
    }

    int i;
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (!node)
        return INSERTION_ERROR;

    // insertion in the head of the queue
    if (pos == 0)
    {
        node->next = queue->pout;
        node->data = value;
        queue->pout = node;
        return OK;
    }
    //
    node_t *tmp;
    for (i = 0, tmp = queue->pout; i < pos - 1 && tmp->next; i++, tmp = tmp->next)
        ;
    node->next = tmp->next->next;
    tmp->next = node;
    node->data = value;
    return OK;
}

static void ring_array_insert(queue_a *const queue, const int value, const int pos)
{
    int i = (queue->pin > pos) ? queue->pin : queue->pin + QUEUE_SIZE;
    while (i > pos)
    {
        queue->data[i - 1] = queue->data[i];
        i--;
    }
    queue->pin++;
    queue->data[i] = value;
}

// this functions inserts the value element on the pos position in the
// array queue (or in the end of the queue if pos >= queue size)
int ins_a(queue_a *const queue, const int value, const int pos)
{
    int err_code;
    // remove last element of the queue if the queue is full (to insert new element)
    if (!queue->is_empty && queue->pin == queue->pout)
    {
        int *val = NULL;
        pop_a(queue, val);
        err_code = OVERFLOW_ERROR;
    }
    else
        err_code = OK;
    int i;
    for (i = 0; i < pos && queue->pout + i < queue->pin; i++)
        ;
    ring_array_insert(queue, value, i);
    return err_code;
}

// this function appends new element in array-queue if it possible
// returns 0 if successful else error code
int push_a(queue_a *const queue, const int value)
{
    if (!queue->is_empty && queue->pin == queue->pout)
        return OVERFLOW_ERROR;
    queue->data[queue->pin] = value;
    queue->pin = (queue->pin + 1) % QUEUE_SIZE;
    queue->is_empty = 0;
    return OK;
}

// this fucntion receives queue head, and return 0, if value is read
// (and also moves head to the next element)
// else error code (value returns with argument)
int pop_l(queue_l *const queue, int *const value)
{
    if (queue->size <= 0)
        return EMPTY_QUEUE;

    *value = queue->pout->data;
    node_t *tmp = queue->pout;
    queue->pout = queue->pout->next;
    free(tmp);
    queue->size--;
    return OK;
}

// this function moves pout index-pointer if it possible
// returns 1 if successful else error code
// (returns value using argument as functions higher)
int pop_a(queue_a *const queue, int *const value)
{
    if (queue->is_empty)
        return EMPTY_QUEUE;

    *value = queue->data[queue->pout];
    queue->pout = (queue->pout + 1) % QUEUE_SIZE;
    if (queue->pout == queue->pin)
        queue->is_empty = 1;
    return OK;
}
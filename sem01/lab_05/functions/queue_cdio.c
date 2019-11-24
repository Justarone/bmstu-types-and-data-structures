#include "queue_cdio.h"
#include <stdlib.h>

#define OK 0
#define OVERFLOW_ERROR 1
#define EMPTY_QUEUE 2

// this function receives pointer on the tail of the queue and appends
// one more element (returns new tail if successful else NULL)
// DON'T FOFGET TO SET POUT IF ITS NULL
queue_l *push_l(queue_l *const queue, const double value)
{
    if (queue->size > QUEUE_SIZE)
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

// this function appends new element in array-queue if it possible
// returns 0 if successful else no
int push_a(queue_a *const queue, const double value)
{
    if (!queue->is_empty)
        if (queue->pin == queue->pout)
            return OVERFLOW_ERROR;
    queue->data[queue->pin] = value;
    queue->pin = (queue->pin + 1) % QUEUE_SIZE;
    queue->is_empty = 0;
    return OK;
}

// this fucntion receives queue head, and return 0, if value is read
// (and also moves head to the next element)
// else error code (value returns with argument)
int pop_l(queue_l *const queue, double *const value)
{
    if (queue->size <= 0)
        return EMPTY_QUEUE;

    *value = queue->pout->data;
    queue_l *tmp = queue->pout;
    queue->pout = queue->pout->next;
    free(tmp);
    queue->size--;
    return OK;
}

// this function moves pout index-pointer if it possible
// returns 1 if successful else error code
// (returns value using argument as functions higher)
int pop_a(queue_a *const queue, double *const value)
{
    if (queue->is_empty)
        return EMPTY_QUEUE;

    *value = queue->data[queue->pout];
    queue->pout = (queue->pout + 1) % QUEUE_SIZE;
    if (queue->pout == queue->pin)
        queue->is_empty = 1;
    return OK;
}
#ifndef _QUEUE_H_
#define _QUEUE_H_

// the queue, which is using list will look like just node
// (I am going to realize interface, where I store just the
// pointer on the first element in the queue)
typedef struct queue_l
{
    double data;          // element data
    struct queue_l *next; // pointer on the next element of the queue
} queue_l;

typedef struct
{
    double *data;  // pointer on the dynamic array, which contains elements of the queue
    int data_size; // size of array with the queue
    double *pin;   // pointer on the place to come in the queue
    double *pout;  // pointer on the place to come out from the queue
} queue_a;

#endif
#ifndef _STACK_H_
#define _STACK_H_

typedef struct node_t
{
    void *value;         // value of the node data
    struct node_t *next; // pointer on the next node (NULL if the last node)
} node_t;

typedef struct
{
    void **data; // the beginning of stack (like the bottom)
    void **last; // the last element, which was added
} stack_a;

#endif
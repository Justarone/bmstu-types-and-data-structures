#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef int ntype;

typedef struct node_t
{
    ntype data;
    struct node_t *next;
} node_t;

#endif
#ifndef _TREE_H_
#define _TREE_H_
// struct for binary tree (searching binary tree)
typedef struct node_t
{
    char *value;
    struct node_t *left;
    struct node_t *right;
} node_t;

#endif
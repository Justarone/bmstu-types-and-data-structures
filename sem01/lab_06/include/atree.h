#ifndef _ATREE_H_
#define _ATREE_H_

// struct of the vertex(es)
typedef struct anode_t
{
    char *value;         // значение в вершине
    unsigned int height; // высота дерева в данной вершине
    struct anode_t *left;
    struct anode_t *right;
} anode_t;

anode_t *remove(anode_t *p, const char *const value); // удаление ключа k из дерева p
anode_t *insert(anode_t *p, const char *const value); // вставка ключа k в дерево с корнем p

#endif
#include "atree.h"
#include <stdlib.h>
#include <string.h>

anode_t *create_node(const char *const value)
{
    anode_t *node = (anode_t *)malloc(sizeof(anode_t));
    if (!node)
        return NULL;

    node->height = 1;
    node->left = (node->right = NULL);
    node->value = (char *)malloc((strlen(value) + 1) * sizeof(char));
    if (!node->value)
    {
        free(node);
        return NULL;
    }
    return node;
}

static void free_node(anode_t *node)
{
    free(node->value);
    free(node);
}

// возвращает высоту, если послали ненулевой указатель, а
// если нулевой, то высота 0
static unsigned int height(anode_t *p)
{
    return p ? p->height : 0;
}

// расчет коэффициента балансировки
static int balance_factor(anode_t *p)
{
    return height(p->right) - height(p->left);
}

void fix_height(anode_t *p)
{
    unsigned int hl = height(p->left);
    unsigned int hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

// функция возвращает новую вершину
// поворот направо вокруг p
static anode_t *rotate_right(anode_t *p)
{
    anode_t *q = p->left;
    p->left = q->right;
    q->right = p;
    fix_height(p);
    fix_height(q);
    return q;
}

// функция возвращает новую вершину
// поворот налево вокруг q
static anode_t *rotate_left(anode_t *q)
{
    anode_t *p = q->right;
    q->right = p->left;
    p->left = q;
    fix_height(q);
    fix_height(p);
    return p;
}

//
static anode_t *balance(anode_t *p)
{
    fix_height(p);

    if (balance_factor(p) == 2)
    {
        if (balance_factor(p->right) < 0)
            p->right = rotate_right(p->right);
        return rotate_left(p);
    }

    else if (balance_factor(p) == -2)
    {
        if (balance_factor(p->left) > 0)
            p->left = rotate_left(p->left);
        return rotate_right(p);
    }
    // балансировка не нужна
    return p;
}

anode_t *insert(anode_t *p, const char *const value) // вставка ключа k в дерево с корнем p
{
    if (!p)
        return create_node(value);

    if (strcmp(p->value, value) < 0)
        p->left = insert(p->left, value);

    else if (strcmp(p->value, value) > 0)
        p->right = insert(p->right, value);

    return balance(p);
}

static anode_t *find_min(anode_t *p) // поиск узла с минимальным ключом в дереве p
{
    return p->left ? find_min(p->left) : p;
}

static anode_t *remove_min(anode_t *p) // удаление узла с минимальным ключом из дерева p
{
    if (!p->left)
        return p->right;
    p->left = remove_min(p->left);
    return balance(p);
}

anode_t *remove(anode_t *p, const char *const value) // удаление ключа value из дерева p
{
    if (!p)
        return NULL;

    if (strcmp(p->value, value) < 0)
        p->left = remove(p->left, value);

    else if (strcmp(p->value, value) > 0)
        p->right = remove(p->right, value);

    else //  k == p->key
    {
        anode_t *q = p->left;
        anode_t *r = p->right;
        free_node(p);
        if (!r)
            return q;
        anode_t *min = find_min(r);
        min->right = remove_min(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}

void clean_atree(anode_t *vertex)
{
    while (vertex = remove(vertex, vertex->value))
        ;
}
#include "calculators.h"

void counter(const node_t *const vertex, const int depth, int *const sum, int *const num)
{
    if (!vertex)
        return;
    counter(vertex->right, depth + 1, sum, num);
    *sum += depth;
    (*num)++;
    counter(vertex->left, depth + 1, sum, num);
}

double count_cmp_tree(const node_t *const vertex)
{
    int sum = 0, num = 0;
    counter(vertex, 1, &sum, &num);
    return (num > 0) ? (double)sum / num : 0;
}

double count_cmp_hash(const hash_t *const table)
{
    if (!table->data)
        return 0;
    int cmp_sum = 0, cmp_num = 0;
    node_h *elem;
    for (int i = 0; i < table->base; i++)
    {
        elem = table->data[i];
        if (!elem)
            continue;
        cmp_sum++, cmp_num++;
        for (int i = 2; elem->next; elem = elem->next, i++)
            cmp_sum += i, cmp_num++;
    }
    return cmp_num == 0 ? 0 : (double)cmp_sum / cmp_num;
}

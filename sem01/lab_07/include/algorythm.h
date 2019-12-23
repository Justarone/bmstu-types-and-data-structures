#ifndef _ALGORYTHM_H_
#define _ALGORYTHM_H_

int dgs(const node_t **const adj_list, const int size,
        ntype **mb_sols, int *const mb_sols_size, const ntype start);

int simple_dgs(const node_t **adj_list, const int size, const ntype start,
               const int avnum);

#endif
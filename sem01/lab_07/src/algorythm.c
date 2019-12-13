#include "struct.h"
#include "stack.h"
#include <stdlib.h>
#include <stdint.h>

// this macros gets
// 1. where - pointer on array in which i store bits:
// if bit = 1, then the vertex is in the tree, else - not
// 2. position to check
// 3. type size - I need it to find right zone to check my bit
#define GET(WHERE, POS, FIELD_SIZE) \
    *(WHERE + POS / FIELD_SIZE) & (1 << POS % FIELD_SIZE)
// same as get but it sets the value of bit on pos position
#define SET(WHERE, POS, FIELD_SIZE) \
    *(WHERE + POS / FIELD_SIZE) | (1 << POS % FIELD_SIZE)
#define OK 0
#define MEM_ERROR 17

typedef uint64_t mtype;

// dgs - depth graph search
// adj_list - list with edges (for every vertex there is vertexes,
// which are connected to it)
// back_edges - array with pairs of vertexes, which are back edge
int dgs(const node_t **const adj_list, const int size,
        ntype **back_edges, const ntype start)
{
    // init stack
    node_t *stack = NULL;
    node_t *tmp = push(stack, start);
    if (!tmp)
        return MEM_ERROR;
    stack = tmp;

    mtype *checked = (mtype *)calloc(size / sizeof(mtype) + 1, sizeof(mtype));
}

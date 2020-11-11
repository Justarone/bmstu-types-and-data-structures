#include "struct.h"
#include "stack.h"
#include <stdlib.h>
#include <stdint.h>
#include "algorythm.h"
#include <stdio.h>

#define OK 0
#define FORREST_ERROR 1
#define NO_SOLUTION 2
#define MEM_ERROR 17

typedef uint64_t mtype;

// dgs - depth graph search
// adj_list - list with edges (for every vertex there is vertexes,
// which are connected to it)
// back_edges - array with pairs of vertexes, which are back edge
int dgs(node_t **adj_list, const int size,
        ntype **mb_sols, int *const mb_sols_size, const ntype start)
{
    // additional variables
    ntype current = 0;
    node_t *elem;
    // init stack
    node_t *stack = NULL;

    node_t *tmp = push(stack, 0);
    if (!tmp)
        return MEM_ERROR;
    stack = tmp;

    tmp = push(stack, start);
    if (!tmp)
        return MEM_ERROR;
    stack = tmp;

    // mtype was created for bit operations, but they were deleted for now
    mtype *checked = (mtype *)calloc(size, sizeof(mtype));
    // var for route length and array for route vertexes
    int rlen = 0;
    ntype *route = (ntype *)calloc(size, sizeof(ntype));
    // array to write, how many cycles goes through the vertexes
    ntype *cycle_index = (ntype *)calloc(size, sizeof(ntype));
    // пока стек не пуст
    while (stack)
    {
        // достаем из стека текущую вершину и рассматриваем его список смежности
        current = pop(&stack);
        rlen = pop(&stack);
        if (checked[current])
            continue;
        route[rlen++] = current;
        checked[current] = 1;
        elem = adj_list[current];
        while (elem)
        {
            // если текущая вершина встречена впервые
            if (!checked[elem->data])
            {
                tmp = push(stack, rlen);
                if (!tmp)
                {
                    printf("error occured in stack push, aborting.");
                    return MEM_ERROR;
                }
                stack = tmp;

                tmp = push(stack, elem->data);
                if (!tmp)
                {
                    printf("error occured in stack push, aborting.");
                    return MEM_ERROR;
                }
                stack = tmp;
            }
            // если вершина уже была, то мы нашли обратное ребро
            // добавляем его в список обратных ребер
            else if (rlen > 1 && elem->data != route[rlen - 2])
            {
                int i;
                for (i = 0; route[i] != elem->data; i++)
                    ;
                for (; i < rlen; i++)
                    cycle_index[route[i]]++;
            }
            elem = elem->next;
        }
    }
    // проверка, все ли вершины проверены (иначе получен лес)
    for (int i = 0; i < size / sizeof(mtype); i++)
        if (!checked[i])
            return FORREST_ERROR;

    int max_index = -1, count = 0;
    for (int i = 0; i < size; i++)
    {
        if (cycle_index[i] > max_index)
        {
            count = 1;
            max_index = cycle_index[i];
        }

        else if (cycle_index[i] == max_index)
            count++;
    }
    if (max_index > 0)
    {
        *mb_sols_size = count;
        *mb_sols = (int *)calloc(size, sizeof(int));

        int j = 0;
        for (int i = 0; i < size; i++)
            if (cycle_index[i] == max_index)
                (*mb_sols)[j++] = i;
    }

    free(checked);
    free(route);
    free(cycle_index);
    return OK;
}

void dig(node_t **adj_list, int *const checked, const int number, const int prev)
{
    if (checked[number])
        return;
    node_t *elem = adj_list[number];
    checked[number] = 1;
    while (elem)
    {
        if (checked[elem->data] && elem->data != prev)
            checked[elem->data] = 2;
        dig(adj_list, checked, elem->data, number);
        elem = elem->next;
    }
}

int simple_dgs(node_t **adj_list, const int size, const ntype start, const int avnum)
{
    int *checked = (int *)calloc(size, sizeof(int));
    dig(adj_list, checked, start, -1);
    for (int i = 0; i < size; i++)
        if (checked[i] != 1 && i != avnum)
        {
            free(checked);
            return FORREST_ERROR;
        }
    free(checked);
    return OK;
}

// solution is array of size 2 (to write two vertexes for depth search)
// returns OK if we can make tree else error
// int check_vertex(const ntype **const array, const int size, int *const solution)
// {
//     if (size == 0)
//         return NO_SOLUTION;

//     solution[0] = array[0][0];
//     solution[1] = array[0][1];

//     for (int i = 1; i < size && (solution[0] != -1 || solution[1] != -1); i++)
//     {
//         if (solution[0] != array[i][0] && solution[0] != array[i][1])
//             solution[0] = -1;

//         if (solution[1] != array[i][0] && solution[1] != array[i][1])
//             solution[1] = -1;
//     }

//     if (solution[0] == -1 && solution[1] == -1)
//         return NO_SOLUTION;

//     return OK;
// }
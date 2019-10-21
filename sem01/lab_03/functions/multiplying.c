#include "multiplying.h"

#define OK 0
#define MULT_ERROR -1

static int ip_factor[MAX_SIZE_MATRIX];
static int ip_res[MAX_SIZE_MATRIX];

int multiply_t(const matrix_t *const matrix, const matrix_t *const factor,
               matrix_t *const res)
{
    // if the condition is false we can't multiply matrix on vector because of incorrect sizes
    if (matrix->columns != factor->rows || matrix->rows != res->rows)
        return MULT_ERROR;

    res->quantity = 0;
    // initialization of ip arrays
    for (int i = 0; i < matrix->rows; i++)
        ip_res[i] = -1;
    for (int i = 0; i < factor->rows; i++)
        ip_factor[i] = -1;

    // setting up the ip_factor array using vector
    for (int i = 0; i < factor->quantity; i++)
        ip_factor[factor->row[i]] = i; // so, our vector shouldn't be ordered,
    // so we need ip array to get position value for O(1)

    l_list_elem pointer = matrix->pointer[0];
    int cur_col = 0;
    /* the multiplication algorithm is:
    as we know, every element with indexes i, j from matrix (matrix[i][j])
    in multiplication with element from vector with index j (vector[k]) 
    is using in sum for element from result matrix with index i

    so we can use all elements of the matrix (if we will explore the columns with 
    columns' pointers, we will know about the columns and rows both, so knowledge 
    about the column will help us with multiplication with right elem from factor
    vector and the knowledge about the row will help us to find, where we need to 
    add this multi) and the time complexity will be proportional to the number of
    elements in the array
     */

    // while we didn't reach the pointer behind the last column
    while (pointer.next)
    {
        // if the element from the factor vector is not zero
        // (with which we are going to multiply our matrix column)
        if (ip_factor[cur_col] != -1)
        {
            // explore all elements from the column of our matrixf
            for (int i = pointer.index; i < pointer.next->index; i++)
            {
                // if the next non-zero sum position in res vector is not defined
                // define it and put the result
                if (ip_res[matrix->row[i]] == -1)
                {
                    ip_res[matrix->row[i]] = res->quantity;
                    res->row[res->quantity] = matrix->row[i];
                    res->value[res->quantity] =
                        factor->value[ip_factor[cur_col]] * matrix->value[i];
                    res->quantity++;
                    res->pointer->next->index++;
                }
                // if the sum position defined in res matrix - update value
                else
                    res->value[ip_res[matrix->row[i]]] +=
                        factor->value[ip_factor[cur_col]] * pointer.index;
            }
        }
        // go to the next column of the matrix and move list element
        cur_col++;
        pointer.index = pointer.next->index;
        pointer.next = pointer.next->next;
    }
    return OK;
}

int multiply_std(const matrix_std *const matrix, const matrix_std *const factor,
                 matrix_std *const res)
{
    if (matrix->columns != factor->rows || matrix->rows != res->rows)
        return MULT_ERROR;

    for (int i = 0; i < matrix->rows; i++)
        for (int j = 0; j < matrix->columns; j++)
            res->data[j][0] += matrix->data[j][i] * factor->data[i][0];
    return OK;
}

//     // the main condition that allows us to multiply matrices (sizes check)
//     if (matrix->columns != factor->size || matrix->rows != res->size)
//         return MULT_ERROR;

//     res->quantity = 0;

//     // initialization of ip arrays
//     for (int i = 0; i < matrix->rows; i++)
//         ip_res[i] = -1;
//     for (int i = 0; i < factor->size; i++)
//         ip_factor[i] = -1;

//     // set positions of non-zero elems of factor vector in ip_factor array
//     for (int i = 0; i < factor->quantity; i++)
//         ip_factor[factor->positions[i]] = i;
//     // go rows
//     for (int j = 0; j < matrix->rows; j++)
//         // go columns
//         for (int i = 0; i < matrix->columns; i++)
//             // if elements of the matrix and array, which are multiplied,
//             // are non-zero so continue
//             if (ip_factor[i] != -1 || matrix->data[j][i] != 0)
//             {
//                 if (ip_res[j] == -1)
//                 {
//                     res->positions[res->quantity] = j;
//                     res->values[res->quantity] = matrix->data[j][i] *
//                                                  factor->values[ip_factor[i]];

//                     ip_res[j] = res->quantity;
//                     res->quantity++;
//                 }
//                 else
//                     res->values[ip_res[j]] += matrix->data[j][i] *
//                                               factor->values[ip_factor[i]];
//             }
// }
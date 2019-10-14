#include "../headers/multiplying.h"
#include "../headers/matrix_struct.h"

static int ip_factor[MAX_SIZE_MATRIX];
static int ip_res[MAX_SIZE_MATRIX];

int multiply_t(const matrix_t *const matrix, const vector_t *const factor, const vector_t *const res)
{
    // initialization of ip arrays
    for (int i = 0; i < matrix->rows; i++)
        ip_res[i] = -1;
    for (int i = 0; i < factor->size; i++)
        ip_factor[i] = -1;

    // setting up the ip_factor array using vector
    for (int i = 0; i < factor->quantity; i++)
        ip_factor[factor->positions[i]] = i;

    l_list_elem pointer = matrix->pointer[0];
    for (int i = pointer.index; i < (pointer.next).index; i++)
    {
        while (cur < matrix->pointer)
    }

}
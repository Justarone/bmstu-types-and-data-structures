#ifndef _MULTIPLYING_H_
#define _MULTIPLYING_H_

#include "matrix_struct.h"

int multiply_t(const matrix_t *const matrix, const matrix_t *const factor,
               matrix_t *const res);
int multiply_std(const matrix_std *const matrix, const matrix_std *const factor,
                 matrix_std *const res);

#endif
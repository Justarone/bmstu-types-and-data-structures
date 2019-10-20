#ifndef _MATRIX_CDIO_H_
#define _MATRIX_CDIO_H_

#include "matrix_struct.h"
#include <stdio.h>

int create_matrix(matrix_t *const matrix, const int rows, const int columns);
int create_matrix_std(matrix_std *const matrix, const int rows, const int columns);
int input_matrix_std(matrix_std *const matrix, FILE *const stream, const int count);
int input_matrix(matrix_t *const matrix, FILE *const stream, const int count);
void convert_matrix(const matrix_t *const source, matrix_std *const target);
void print_matrix(const matrix_std *const matrix, FILE *const stream);

#endif
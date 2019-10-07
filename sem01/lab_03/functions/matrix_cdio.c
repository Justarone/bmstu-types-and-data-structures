#include "../headers/matrix_cdio.h"

#define SIZE_ERROR 17
#define OK 0
#define COUNT_ERROR 18
#define READ_ERROR 19
#define READED 1
#define TRIPLE_READED 3

int create_matrix(matrix_t *const matrix, const int rows, const int columns)
{
    if (rows <= 0 || columns <= 0 || rows > MAX_SIZE_MATRIX || columns > MAX_SIZE_MATRIX)
        return SIZE_ERROR;
    matrix->rows = rows;
    matrix->columns = columns;
    return OK;
}

int create_matrix_std(matrix_std *const matrix, const int rows, const int columns)
{
    if (rows <= 0 || columns <= 0)
        return SIZE_ERROR;
    matrix->rows = rows;
    matrix->columns = columns;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            matrix->data[i][j] = 0;
    return OK;
}

int input_matrix_std(matrix_std *const matrix, FILE *const stream)
{
    // input of nonzero elements count
    int count;
    int triple[3]; // array for triples = { row, column, value }
    if (fscanf(stream, "%d", &count) != READED || count <= 0 || count >= MAX_MATRIX_ELEMS)
        return COUNT_ERROR;
    for (int i = 0; i < count; i++)
    {
        if (fscanf(stream, "%d%d%d", triple, triple + 1, triple + 2) != TRIPLE_READED)
            return READ_ERROR;
        matrix->data[triple[0]][triple[1]] = triple[2];
    }
    return OK;
}

int input_matrix_t(matrix_t *const matrix, FILE *const stream)
{
    // input of nonzero elements count
    int count;
    int triple[3]; // array for triples = { row, column, value }
    if (fscanf(stream, "%d", &count) != READED || count <= 0 || count >= MAX_MATRIX_ELEMS)
        return COUNT_ERROR;
    for (int i = 0; i < count; i++)
    {
        if (fscanf(stream, "%d%d%d", triple, triple + 1, triple + 2) != TRIPLE_READED)
            return READ_ERROR;
        
}
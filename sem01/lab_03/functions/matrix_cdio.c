#include "matrix_cdio.h"

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

int input_matrix_std(matrix_std *const matrix, FILE *const stream, const int count)
{
    // input of nonzero elements count
    int triple[3]; // array for triples = { row, column, value }
    if (count <= 0 || count >= MAX_MATRIX_ELEMS)
        return COUNT_ERROR;
    for (int i = 0; i < count; i++)
    {
        if (fscanf(stream, "%d%d%d", triple, triple + 1, triple + 2) != TRIPLE_READED ||
            triple[0] >= matrix->rows || triple[1] >= matrix->columns)
            return READ_ERROR;
        matrix->data[triple[0]][triple[1]] = triple[2];
    }
    return OK;
}

void ins_elem_in_list(matrix_t *const matrix, const int index, const int value, const int row)
{
    for (int i = index; i < matrix->quantity; i++)
    {
        matrix->value[i + 1] = matrix->value[i];
        matrix->row[i + 1] = matrix->row[i];
    }
    matrix->value[index] = value;
    matrix->row[index] = row;
}

void insert_elem_t(matrix_t *const matrix, const int i, const int j, const int value)
{
    // current index is chosen as the first
    // index of the column and since here we are looking for the insertion place
    int cur_index = matrix->pointer[j].index;
    // if we reached the next
    // column or our row became less than the previous row (so if we reached the
    // condition then we find index for insertion)
    while (cur_index <= matrix->pointer[j].next->index &&
           matrix->row[cur_index] < i)
        cur_index++;

    if (matrix->row[cur_index] = i)
    {
        matrix->value[cur_index] = value;
        return;
    }

    // move all column pointers to the left,
    // cause we are gonna to insert new elem in column with index = j (so after j all pointer's indexes++)
    for (int k = j + 1; k < matrix->columns + 1; k++)
        matrix->pointer[k].index++;
    // insertion of the element if it still doesn't exists
    ins_elem_in_list(matrix, cur_index, value, i);

    matrix->quantity++;
}

int input_matrix_t(matrix_t *const matrix, FILE *const stream, const int count)
{
    // input of nonzero elements count
    int triple[3]; // array for triples = { row, column, value }
    if (count <= 0 || count >= MAX_MATRIX_ELEMS)
        return COUNT_ERROR;
    for (int i = 0; i < count; i++)
    {
        if (fscanf(stream, "%d%d%d", triple, triple + 1, triple + 2) != TRIPLE_READED ||
            triple[0] >= matrix->rows || triple[1] >= matrix->columns)
            return READ_ERROR;
        insert_elem(matrix, triple[0], triple[1], triple[2]);
    }
    return OK;
}

void convert_matrix(const matrix_t *const source, matrix_std *const target)
{
    target->rows = source->rows;
    target->columns = source->columns;

    l_list_elem pointer = source->pointer[0];
    int cur_col = 0;
    while (pointer.next)
    {
        for (int i = pointer.index; i < pointer.next->index; i++)
            target->data[source->row[i]][cur_col] = source->value[i];
    }
}

void print_matrix(const matrix_std *const matrix, FILE *const stream)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->columns; j++)
            fprintf(stream, "%10d", matrix->data[i][j]);
        fprintf(stream, "\n\n");
    }
}
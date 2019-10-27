#ifndef _MATRIX_STRUCT_H_
#define _MATRIX_STRUCT_H_

#define MAX_SIZE_MATRIX 1000
#define MAX_MATRIX_ELEMS 1000000

// стандартная матрица
typedef struct
{
    int rows;
    int columns;
    int data[MAX_SIZE_MATRIX][MAX_SIZE_MATRIX];
} matrix_std;

// Элемент линейного списка
typedef struct l_list_elem
{
    int index;                // индекс, с которого начинается столбец
    struct l_list_elem *next; // указатель на след столбец
} l_list_elem;

// матрица в разреженном виде
typedef struct
{
    int rows;
    int columns;
    int quantity;                             // количество ненулевых элементов в разреженной матрице
    int value[MAX_MATRIX_ELEMS];              // значения ненулевых элементов
    int row[MAX_MATRIX_ELEMS];                // ряды ненулевых элементов
    l_list_elem pointer[MAX_SIZE_MATRIX + 1]; // указатель на элемент, с которого начинается столбец
} matrix_t;

#endif

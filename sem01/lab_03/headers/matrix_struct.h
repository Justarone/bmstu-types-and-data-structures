#ifndef _MATRIX_STRUCT_H_
#define _MATRIX_STRUCT_H_

#define MAX_SIZE_MATRIX 1000
#define MAX_MATRIX_ELEMS 1000000

typedef struct 
{
    int rows;
    int columns;
    int data[MAX_SIZE_MATRIX][MAX_SIZE_MATRIX];
} matrix_std;

typedef struct
{
    l_list_elem *next;
    int index;  // It's not rational for me to use the real pointer cause I need to get both characterics (value and column)
} l_list_elem;

typedef struct
{
    int rows;
    int columns;
    int quantity;
    int value[MAX_MATRIX_ELEMS];
    int row[MAX_MATRIX_ELEMS]; 
    l_list_elem pointer[MAX_SIZE_MATRIX + 1];
} matrix_t;

typedef struct
{
    int size;
    int quantity;
    int values[MAX_MATRIX_ELEMS];
    int positions[MAX_MATRIX_ELEMS];
} vector_t;


#endif
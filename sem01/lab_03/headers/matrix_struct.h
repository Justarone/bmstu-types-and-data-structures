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

// I want to make this more universal, so the list can store either row pointers or column ones
typedef struct
{
    l_list_elem *next;
    int value; // here is the value of the first elem of row (or column)
    int *colrow; // it means, that here can be either pointer on columns, or on rows
} l_list_elem;

typedef struct
{
    int rows;
    int columns;
    int value[MAX_MATRIX_ELEMS];
    int rowcol[MAX_MATRIX_ELEMS]; // so, first idea is that this array store rows of value's 
    // but it's better to transpose in the same type (i think, because you have matrix_t = trans(matrix_t))
    l_list_elem pointers[MAX_SIZE_MATRIX];
} matrix_t;

#endif
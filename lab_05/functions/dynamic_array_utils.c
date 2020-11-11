#include "dynamic_array_utils.h"
#include <stdlib.h>

#define OK 0
#define ALLOCATION_ERROR 17
#define ELEMENT_ERROR 18
#define SIZE_ERROR 19
#define NOT_IN -1

int allocate_memory(array_d *const array)
{
    // if this is the first time to allocate memory
    // then we will send NULL to realloc function and it will use
    // smthng like malloc (and will allocate place for 2 elements)
    if (array->mem_size == 0)
    {
        array->data = 0;
        array->mem_size = 1;
    }
    void **temp = (void **)realloc(array->data, array->mem_size * 2 * sizeof(void *));
    if (!temp)
        return ALLOCATION_ERROR;
    array->data = temp;
    array->mem_size *= 2;
    return OK;
}

void swap_p(void **const p1, void **const p2)
{
    void *temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

// this function inserts free spaces in order (like iteration of insertion sort)
int insert_element(array_d *const array, void *const element)
{
    if (array->data_size == array->mem_size)
        if (allocate_memory(array))
            return ALLOCATION_ERROR;

    if (element == NULL)
        return ELEMENT_ERROR;
    array->data[array->data_size++] = element;
    for (int i = array->data_size - 1; array->data[i] < array->data[i - 1] && i > 0; i--)
        swap_p(array->data + i, array->data + i - 1);
    return OK;
}

int delete_element(array_d *const array, const int pos)
{
    if (array->data_size <= 0)
        return SIZE_ERROR;
    for (int i = pos; i < array->data_size - 1; i++)
        array->data[i] = array->data[i + 1];
    array->data_size--;
    return OK;
}

int is_in(array_d *const array, const void *const elem)
{
    int left = 0;
    int right = array->data_size - 1;
    int middle;
    while (left <= right)
    {
        middle = (left + right) / 2;
        if (elem == array->data[middle])
            return middle;
        else if (elem < array->data[middle])
            right = middle - 1;
        else
            left = middle + 1;
    }
    return NOT_IN;
}
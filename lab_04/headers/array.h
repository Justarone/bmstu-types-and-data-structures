#ifndef _ARRAY_H_
#define _ARRAY_H_

// struct for dynamic array with memory manager
typedef struct
{
    void **data;
    int data_size;
    int mem_size;
} array_d;

#endif
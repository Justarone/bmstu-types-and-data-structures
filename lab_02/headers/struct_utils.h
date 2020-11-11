#ifndef _STRUCT_UTILS_H_
#define _STRUCT_UTILS_H_

#include <stdio.h>
#include "student_struct.h"

int add_student(student_t *const stud_list, int *const size, FILE *const f);
int delete_student(student_t *const stud_list, int *const size);
void quick_sort_list(student_t *const stud_list, const int begin, const int end, const int sign);
void bubble_sort_list(student_t *const stud_list, const int size, const int sign);
void quick_sort_keys(keys_t *const keys, const int begin, const int end);
void bubble_sort_keys(keys_t *const keys, const int size);

#endif
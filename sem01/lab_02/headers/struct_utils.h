#ifndef _STRUCT_UTILS_H_
#define _STRUCT_UTILS_H_

#include <stdio.h>
#include "student_struct.h"

int add_student(student_t *const stud_list, int *const size, FILE *const f);
int delete_student(student_t *const stud_list, int *const size, int *const keys);
void quick_sort_list(student_t *const stud_list, const int begin, const int end, const int sign);
void bubble_sort_list(student_t *const stud_list, const int size,  const int sign);
void sort_by_keys(const student_t *const stud_list, int *const keys, const int start, const int end, const int sign);

#endif
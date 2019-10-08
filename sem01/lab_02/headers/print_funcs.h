#ifndef _PRINT_FUNCS_H_
#define _PRINT_FUNCS_H_
#include "student_struct.h"
#include <time.h>

int choose_property();
void print_menu(void);
int print_compare(const double *const times); // Error when less than 2 "times" elements defined
void print_by_year(const student_t *const stud_list, const int size, const int year);
void print_all(const student_t *const stud_arr, const int size);
void print_keys_array(const keys_t *const key_array, const int size);
#endif
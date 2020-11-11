#ifndef _READ_H_
#define _READ_H_
#include "student_struct.h"
#include <stdio.h>

int read_from_file(student_t *const stud_list, const int max_size, FILE *const stream);

#endif
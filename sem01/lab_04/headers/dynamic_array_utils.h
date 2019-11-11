#ifndef _D_ARRAY_UTILS_H_
#define _D_ARRAY_UTILS_H_

#include "array.h"

int insert_element(array_d *const array, const void *const element);
int delete_element(array_d *const array, const int pos);
int is_in(array_d *const array, const void *const elem);

#endif
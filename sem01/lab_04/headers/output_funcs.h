#ifndef _OUTPUT_FUNCS_H_
#define _OUTPUT_FUNCS_H_

#include "stack.h"
#include "array.h"

void print_menu();
void print_stack_l(const node_t *ps); // this function prints list stack
void print_stack_a(stack_a *ps);      // this function prints array stack
void print_free_zones(const array_d *const free_zones);
void print_stat();

#endif
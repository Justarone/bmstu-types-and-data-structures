#ifndef _STACK_CDIO_H_
#define _STACK_CDIO_H_

#include "stack.h"
#include "array.h"

node_t *push_st_l(const node_t *const ps, array_d *const free_zones);
int push_st_a(const stack_a *const ps, const void *const value,
              array_d *const free_zones);
node_t *add_st_l(node_t *const ps, const int n, array_d *const free_zones);
int add_st_a(const stack_a *const ps, const int n, array_d *const free_zones);
void *pop_l(node_t *ps, array_d *const free_zones);
void *pop_a(stack_a *const array_stack, array_d *const free_zones);
int cleann_l(node_t *ps, const int n, array_d *const free_zones);
int cleann_a(stack_a *const array_stack, const int n, array_d *const free_zones);

#endif
#ifndef _STACK_CDIO_H_
#define _STACK_CDIO_H_

#include "stack.h"
#include "array.h"

node_t *push_st_l(node_t *const ps);
int push_st_a(stack_a *const ps, void *const value);
// Нужна временная обертка и проверка добавленных элементов
node_t *add_st_l(node_t **const ps, int *const n,
                 long int *const add_time);
// посылаем время аргументом!
int add_st_a(stack_a *const ps, int *const n, const int mode,
             long int *const add_time);
void *pop_l(node_t **ps);
void *pop_a(stack_a *const array_stack);
// посылаем время аргументом
int cleann_l(node_t **ps, const int n, long int *const clean_time,
             array_d *const free_zones);
int cleann_a(stack_a *const array_stack, const int n, long int *const clean_time);

#endif
#ifndef _QUEUE_CDIO_H_
#define _QUEUE_CDIO_H_

#include "queue.h"
#include "array.h"

queue_l *push_st_l(queue_l *const ps, array_d *const free_zones);
int push_st_a(queue_a *const ps, void *const value,
              array_d *const free_zones);
void *pop_l(queue_l **ps, array_d *const free_zones);
void *pop_a(queue_a *const array_stack, array_d *const free_zones);
#endif
#ifndef _QUEUE_CDIO_H_
#define _QUEUE_CDIO_H_

#include "queue.h"
#include "array.h"

queue_l *push_l(queue_l *const qh, const double value);
int push_a(queue_a *const ps, const double value,
           array_d *const free_zones);
void *pop_l(queue_l **ps, array_d *const free_zones);
void *pop_a(queue_a *const array_stack, array_d *const free_zones);
#endif
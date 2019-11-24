#ifndef _QUEUE_CDIO_H_
#define _QUEUE_CDIO_H_

#include "queue.h"
#include "array.h"

queue_l *push_l(queue_l *const qh, const double value);
int push_a(queue_a *const ps, const double value);
int pop_l(queue_l *const queue, double *const value);
int pop_a(queue_a *const queue, double *const value);

#endif
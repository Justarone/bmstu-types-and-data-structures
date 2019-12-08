#ifndef _QUEUE_CDIO_H_
#define _QUEUE_CDIO_H_

#include "queue.h"
#include "array.h"

node_t *push_l(queue_l *const qh, const int value);
int push_a(queue_a *const ps, const int value);
int ins_l(queue_l *const queue, const double value, const int pos, void **const address, const double income_time);
int ins_a(queue_a *const queue, const int value, const int pos, const double income_time);
int pop_l(queue_l *const queue, int *const value);
int pop_a(queue_a *const queue, int *const value);
// void print_queue(queue_l *const queue);
void print_queue(const queue_a *const queue);
void print_queue_l(queue_l *const queue);
#endif
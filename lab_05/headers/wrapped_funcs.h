#ifndef _WRAPPED_FUNCS_H_
#define _WRAPPED_FUNCS_H_

#include "queue.h"
#include "array.h"
#include "additional_structs.h"

void wrapped_push_l(queue_l *const queue, stat_t *const statistic,
                    timer_t *const pp_timer, array_d *const free_zones,
                    const int value);
void wrapped_ins_l(queue_l *const queue, stat_t *const statistic,
                   timer_t *const pp_timer, array_d *const free_zones,
                   const int pos, const int value);
int wrapped_pop_l(queue_l *const queue, stat_t *const statistic,
                  timer_t *const pp_timer, array_d *const free_zones,
                  int *const value);
int wrapped_push_a(queue_a *const queue, stat_t *const statistic,
                   timer_t *const pp_timer, const int value);
void wrapped_ins_a(queue_a *const queue, stat_t *const statistic,
                   timer_t *const pp_timer, const int pos, const int value);
int wrapped_pop_a(queue_a *const queue, stat_t *const statistic,
                  timer_t *const pp_timer, int *const value);

#endif
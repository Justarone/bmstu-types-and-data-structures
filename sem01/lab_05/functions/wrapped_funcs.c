#include "wrapped_funcs.h"
#include "timer.h"
#include "queue.h"
#include "queue_cdio.h"
#include "dynamic_array_utils.h"

#define OK 0

// variables to check fragmentation
int freed_zone = 0;
int new_zone = 0;

void wrapped_push_l(queue_l *const queue, stat_t *const statistic,
                    timer_t *const pp_timer, array_d *const free_zones,
                    const int value)
{
    node_t *tmp;
    uint64_t start = tick();
    tmp = push_l(queue, value);
    if (tmp)
    {
        queue->pin = tmp;
        // updating info for average queue length
        statistic->len_sum += queue->size;
        statistic->check_num++;
        // ending measuring time and updating stat
        pp_timer->time[PUSH] += tick() - start;
        pp_timer->opers_count[PUSH]++;
        int pos;
        if ((pos = is_in(free_zones, tmp)))
        {
            delete_element(free_zones, pos);
            freed_zone++;
        }
        else
            new_zone++;
    }
    else
        statistic->lost_apps++;
}

void wrapped_ins_l(queue_l *const queue, stat_t *const statistic,
                   timer_t *const pp_timer, array_d *const free_zones,
                   const int pos, const int value)
{
    int err_code = ins_l(queue, value, pos);
    if (err_code != OK)
    {
        // updating info for average queue length
        statistic->len_sum += queue->size;
        statistic->check_num++;
        // ending measuring time and updating stat???
    }
    else
        statistic->lost_apps++;
}

int wrapped_pop_l(queue_l *const queue, stat_t *const statistic,
                  timer_t *const pp_timer, array_d *const free_zones,
                  int *const value)
{
    uint64_t start = tick();
    void *address = queue->pout;
    int err_code = pop_l(queue, value);
    if (err_code == OK)
    {
        // updating info for average queue length
        statistic->len_sum += queue->size;
        statistic->check_num++;
        // ending measuring time and updating stat
        pp_timer->time[POP] += tick() - start;
        pp_timer->opers_count[POP]++;
        insert_element(free_zones, address);
    }
    return err_code;
}

int wrapped_push_a(queue_a *const queue, stat_t *const statistic,
                   timer_t *const pp_timer, const int value)
{
    uint64_t start = tick();
    int err_code = push_a(queue, value);
    if (err_code == OK)
    {
        // updating info for average queue length
        statistic->len_sum += (queue->pin > queue->pout) ? queue->pin - queue->pout : queue->pin + QUEUE_SIZE - queue->pout;
        statistic->check_num++;
        // ending measuring time and updating stat
        pp_timer->time[PUSH] += tick() - start;
        pp_timer->opers_count[PUSH]++;
    }
    else
        statistic->lost_apps++;
    return err_code;
}

void wrapped_ins_a(queue_a *const queue, stat_t *const statistic,
                   timer_t *const pp_timer, const int pos, const int value)
{
    int err_code = ins_a(queue, value, pos);
    if (err_code == OK)
    {
        // updating info for average queue length
        statistic->len_sum += (queue->pin > queue->pout) ? queue->pin - queue->pout : queue->pin + QUEUE_SIZE - queue->pout;
        statistic->check_num++;
    }
    else
        statistic->lost_apps++;
}

int wrapped_pop_a(queue_a *const queue, stat_t *const statistic,
                  timer_t *const pp_timer, int *const value)
{
    uint64_t start = tick();
    int err_code = pop_a(queue, value);
    if (err_code == OK)
    {
        // updating info for average queue length
        statistic->len_sum += (queue->pin > queue->pout) ? queue->pin - queue->pout : queue->pin + QUEUE_SIZE - queue->pout;
        statistic->check_num++;
        // ending measuring time and updating stat
        pp_timer->time[POP] += tick() - start;
        pp_timer->opers_count[POP]++;
    }
    return err_code;
}
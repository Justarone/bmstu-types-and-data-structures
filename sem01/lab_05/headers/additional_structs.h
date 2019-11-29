#ifndef _STAT_STRUCT_H_
#define _STAT_STRUCT_H_

#include <stdint.h>

#define TYPES_AMOUNT 2
#define TYPE_1 0
#define TYPE_2 1

#define OPERATIONS_AMOUNT 2
#define PUSH 0
#define POP 1

typedef struct stat_t
{
    // process time
    double process_time;
    // average queue length check
    int len_sum;   // sum of values of the queue, when deleting/adding element
    int check_num; // amount of numbers in sum higher
    // total amount of applications come in
    int in_amount[TYPES_AMOUNT];
    // total time of work of the machine and number of applications
    double work_time[TYPES_AMOUNT]; // total time of work of the machine
    int out_amount[TYPES_AMOUNT];   // amount of done applications
    // time of waiting of the machine
    double waiting_time;
    // lost applications (lost because of full queue or other errors of push function)
    int lost_apps;
} stat_t;

typedef struct timer_t
{
    uint64_t time[OPERATIONS_AMOUNT];
    uint64_t opers_count[OPERATIONS_AMOUNT];
} timer_t;

#endif
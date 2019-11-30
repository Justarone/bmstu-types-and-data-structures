#include "main_cycles.h"
#include "queue_cdio.h"
#include "queue.h"
#include "timer.h"
#include "dynamic_array_utils.h"
#include <stdlib.h>
#include "additional_structs.h"
#include "wrapped_funcs.h"
#include "output_funcs.h"
#include <stdio.h>

// this macros generates random number from BEG to END with some accuracy
// accuracy sets like 0.01, 0.001 etc.
// example: rand(2, 3, 0.001) rerurns 2.342, 2.512 etc
#define ACCURACY 1E-3
#define EPS 1E-7
#define RAND(BEG, END, EPS) (rand() % ((END - BEG) * (int)(1 / EPS))) * EPS
#define MIN(A, B) (A <= B) ? A : B;
#define OK 0
// constants for experiment (ps: BEG, END - borders for rand values)
#define SEED 17
#define INCOME1_BEG 0
#define INCOME1_END 5
#define WORK1_BEG 0
#define WORK1_END 4
#define WORK2_BEG 0
#define WORK2_END 4

#define APPS_AMOUNT 1000
#define PRINT_STEP 100

#define TIMES_AMOUNT 2
#define QUEUE_INCOME_TIME 0
#define MACHINE_WORK_TIME 1

// situations
#define NORMAL 0
#define WAIT_MODE 1

// position to insert the application of type 2
#define POS_2 3

#define SRAND_SEED 17

extern int freed_zone;
extern int new_zone;

stat_t list_cycle(array_d *const free_zones)
{
    // firstly we need to set random to return the same values for both processes
    srand(SRAND_SEED);
    int situation = NORMAL; // helps to classificate situations (like `machine is working`
    // or `machine is waiting`)
    int err_code;                 // type of application and err_code variables
    int last_in_machine = TYPE_1; // this variable stores type of application which has come
    int value;                    // variable to receive value of application
    // (type) from the queue (during popping?))
    // in the machine last time
    queue_l queue = {};
    stat_t statistic = {};

    // initialization of queue: insertion of element of type 2
    timer_t pp_timer = {}; // push-pop timer

    // this is insert function with timer and statistic wrapper
    // printf("before first insert:\n");
    // print_queue(&queue);
    wrapped_ins_l(&queue, &statistic, &pp_timer, free_zones, POS_2, TYPE_2);
    // printf("after first insert:\n");
    // print_queue(&queue);
    statistic.in_amount[TYPE_2]++;
    // for the first time the machine is empty but we count like the first
    // type of application was there, so we need to remove one
    statistic.out_amount[TYPE_1]--;

    double times[TIMES_AMOUNT] = {};
    times[QUEUE_INCOME_TIME] = RAND(INCOME1_BEG, INCOME1_END, ACCURACY);

    int i = 1;
    while (statistic.out_amount[TYPE_1] < APPS_AMOUNT)
    {
        switch (situation)
        {
        case NORMAL:
            // if the time of coming of the new application has come
            if (times[QUEUE_INCOME_TIME] - EPS <= 0)
            {
                statistic.in_amount[TYPE_1]++;
                // updating the time (writing the time of coming of the next application)
                times[QUEUE_INCOME_TIME] = RAND(INCOME1_BEG, INCOME1_END, ACCURACY);
                // pushing new application of type 1
                // printf("before pushing TYPE_1:\n");
                // print_queue(&queue);
                wrapped_push_l(&queue, &statistic, &pp_timer, free_zones, TYPE_1);
                // printf("after pushing type1:\n");
                // print_queue(&queue);
            }
            // if the machine ended its work
            if (times[MACHINE_WORK_TIME] - EPS <= 0)
            {
                statistic.out_amount[last_in_machine]++;
                if (last_in_machine == TYPE_2)
                {
                    statistic.in_amount[TYPE_2]++;
                    // printf("before inserting type2:\n");
                    // print_queue(&queue);
                    wrapped_ins_l(&queue, &statistic, &pp_timer, free_zones, POS_2, TYPE_2);
                    // printf("after inserting type2:\n");
                    // print_queue(&queue);
                }
                // printf("before popping element form the queue:\n");
                // print_queue(&queue);
                err_code = wrapped_pop_l(&queue, &statistic, &pp_timer, free_zones, &value);
                // printf("after popping element from the queue:\n");
                // print_queue(&queue);
                if (err_code != OK)
                {
                    situation = WAIT_MODE;
                    break;
                }
                last_in_machine = value;
                if (value == TYPE_1)
                    times[MACHINE_WORK_TIME] = RAND(WORK1_BEG, WORK1_END, ACCURACY);
                else
                    times[MACHINE_WORK_TIME] = RAND(WORK2_BEG, WORK2_END, ACCURACY);
            }

            double time = MIN(times[MACHINE_WORK_TIME], times[QUEUE_INCOME_TIME]);
            times[MACHINE_WORK_TIME] -= time;
            times[QUEUE_INCOME_TIME] -= time;
            statistic.process_time += time;
            statistic.work_time[last_in_machine] += time;
            break;

        case WAIT_MODE:
            // it the machine is waiting for the next application
            // we just skip the time to the next application appearance
            // and writing this time to the process time and waiting time
            // (we don't need to append new application in the queue, cause it
            // will be appended in the case higher)
            statistic.process_time += times[QUEUE_INCOME_TIME];
            statistic.waiting_time += times[QUEUE_INCOME_TIME];
            times[QUEUE_INCOME_TIME] = 0;

            situation = NORMAL;
            break;
        }

        // we print informations about queue's and stat
        if (statistic.out_amount[TYPE_1] == PRINT_STEP * i)
        {
            i++;
            print_stat(&statistic);
            // printf("\nqueue_size = %d\n", queue.size);
        }
    }
    printf("\n\nВзято из новой области: %d\nВзято из старой области: %d\n\n",
           new_zone, freed_zone);

    // clean list
    node_t *tmp;
    while (queue.pout)
    {
        tmp = queue.pout;
        queue.pout = queue.pout->next;
        free(tmp);
    }
    return statistic;
}

stat_t array_cycle(void)
{
    srand(SRAND_SEED);
    int situation = NORMAL; // helps to classificate situations (like `machine is working`
    // or `machine is waiting`)
    int err_code;                 // type of application and err_code variables
    int last_in_machine = TYPE_1; // this variable stores type of application which has come
    int value = 0;                // variable to receive value of application
    // (type) from the queue (during popping?))
    // in the machine last time
    queue_a queue = {};
    queue.is_empty = 1;
    stat_t statistic = {};

    // initialization of queue: insertion of element of type 2
    timer_t pp_timer = {}; // push-pop timer

    // this is push function with timer and statistic wrapper
    // printf("first insertion application of type2 in queue\n before:\n");
    // print_queue(&queue);
    wrapped_ins_a(&queue, &statistic, &pp_timer, POS_2, TYPE_2);
    // printf("after:");
    // print_queue(&queue);
    statistic.in_amount[TYPE_2]++;
    statistic.out_amount[TYPE_1]--;

    double times[TIMES_AMOUNT] = {};
    times[QUEUE_INCOME_TIME] = RAND(INCOME1_BEG, INCOME1_END, ACCURACY);

    int i = 1;
    while (statistic.out_amount[TYPE_1] < APPS_AMOUNT)
    {
        switch (situation)
        {
        case NORMAL:
            // if the time of coming of the new application has come
            if (times[QUEUE_INCOME_TIME] - EPS <= 0)
            {
                statistic.in_amount[TYPE_1]++;
                // updating the time (writing the time of coming of the next application)
                times[QUEUE_INCOME_TIME] = RAND(INCOME1_BEG, INCOME1_END, ACCURACY);
                // pushing new application of type 1
                // printf("push of the 1 type application\n before:\n");
                // print_queue(&queue);
                wrapped_push_a(&queue, &statistic, &pp_timer, TYPE_1);
                // printf("after:\n");
                // print_queue(&queue);
            }
            // if the machine ended its work
            if (times[MACHINE_WORK_TIME] - EPS <= 0)
            {
                statistic.out_amount[last_in_machine]++;
                if (last_in_machine == TYPE_2)
                {
                    statistic.in_amount[TYPE_2]++;
                    // printf("insertion of the 2 type application not farer than 4 position\n before:\n");
                    // print_queue(&queue);
                    wrapped_ins_a(&queue, &statistic, &pp_timer, POS_2, TYPE_2);
                    // printf("after:\n");
                    // print_queue(&queue);
                }
                // printf("pop of the next element of the queue\nbefore:\n");
                // print_queue(&queue);
                err_code = wrapped_pop_a(&queue, &statistic, &pp_timer, &value);
                // printf("after:\n");
                // print_queue(&queue);
                if (err_code != OK)
                {
                    situation = WAIT_MODE;
                    break;
                }
                last_in_machine = value;
                if (value == TYPE_1)
                    times[MACHINE_WORK_TIME] = RAND(WORK1_BEG, WORK1_END, ACCURACY);
                else
                    times[MACHINE_WORK_TIME] = RAND(WORK2_BEG, WORK2_END, ACCURACY);
            }

            double time = MIN(times[MACHINE_WORK_TIME], times[QUEUE_INCOME_TIME]);
            times[MACHINE_WORK_TIME] -= time;
            times[QUEUE_INCOME_TIME] -= time;
            statistic.process_time += time;
            statistic.work_time[last_in_machine] += time;
            break;

        case WAIT_MODE:
            // it the machine is waiting for the next application
            // we just skip the time to the next application appearance
            // and writing this time to the process time and waiting time
            // (we don't need to append new application in the queue, cause it
            // will be appended in the case higher)
            statistic.process_time += times[QUEUE_INCOME_TIME];
            statistic.waiting_time += times[QUEUE_INCOME_TIME];
            times[QUEUE_INCOME_TIME] = 0;

            situation = NORMAL;
            break;
        }

        // we print informations about queue's and stat
        if (statistic.out_amount[TYPE_1] == PRINT_STEP * i)
        {
            i++;
            print_stat(&statistic);
        }
    }
    return statistic;
}
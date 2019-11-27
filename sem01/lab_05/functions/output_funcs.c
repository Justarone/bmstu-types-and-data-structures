#include "output_funcs.h"
#include <stdio.h>
#include <stdlib.h>

void print_stat(const stat_t *const statistic)
{
    printf("\n\nВремя работы системы: %lf\n", statistic->process_time);
    printf("Средняя длина очереди: %d\n", statistic->len_sum / statistic->check_num);
    printf("Количество вошедших заявок: %d 1 типа и %d 2 типа\n", statistic->in_amount[TYPE_1],
           statistic->in_amount[TYPE_2]);
    printf("Количество вышедших заявок: %d 1 типа и %d 2 типа\n", statistic->out_amount[TYPE_1],
           statistic->out_amount[TYPE_2]);
    printf("За это время машина: работала %lf секунд c 1 типом заявок, %lf секунд"
           " c 2 типом заявок и стояла %lf секунд",
           statistic->work_time[TYPE_1],
           statistic->work_time[TYPE_2],
           statistic->waiting_time);
    printf("Количество потерянных заявок (в связи с переполненной очередью): %d",
           statistic->lost_apps);
}

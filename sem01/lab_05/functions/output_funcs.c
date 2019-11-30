#include "output_funcs.h"
#include <stdio.h>
#include <stdlib.h>

void print_stat(const stat_t *const statistic)
{
       printf("\n\nВремя работы системы: %lf\n", statistic->process_time);
       printf("Средняя длина очереди: %d\n", statistic->len_sum / statistic->check_num);
       printf("Количество вошедших заявок:\n%d 1 типа;\n%d 2 типа\n", statistic->in_amount[TYPE_1],
              statistic->in_amount[TYPE_2]);
       printf("Количество вышедших заявок:\n%d 1 типа;\n%d 2 типа;\n", statistic->out_amount[TYPE_1],
              statistic->out_amount[TYPE_2]);
       printf("За это время машина:\nработала %lf секунд c 1 типом заявок;\nработала %lf секунд"
              " c 2 типом заявок;\nстояла %lf секунд;\n",
              statistic->work_time[TYPE_1],
              statistic->work_time[TYPE_2],
              statistic->waiting_time);
       printf("Среднее время в очереди заявок 1 типа: %lf\nСреднее время в очереди заявок 2 типа: %lf\n",
              statistic->all_queue_times[TYPE_1] / statistic->out_amount[TYPE_1],
              statistic->all_queue_times[TYPE_2] / statistic->out_amount[TYPE_2]);
       printf("Количество потерянных заявок (в связи с переполненной очередью): %d\n",
              statistic->lost_apps);
}

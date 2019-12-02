#include "output_funcs.h"
#include <stdio.h>
#include <stdlib.h>

void print_stat(const stat_t *const statistic)
{
       printf("\n\n\033[1;33mВремя работы системы: \033[1;37m%lf\n", statistic->process_time);
       printf("\033[1;33mСредняя длина очереди: \033[1;37m%d\n", statistic->len_sum / statistic->check_num);
       printf("\033[1;33mКоличество вошедших заявок:\n\033[1;37m%d\033[1;33m 1 типа;\n\033[1;37m%d\033[1;33m 2 типа\n", statistic->in_amount[TYPE_1],
              statistic->in_amount[TYPE_2]);
       printf("\033[1;33mКоличество вышедших заявок:\n\033[1;37m%d\033[1;33m 1 типа;\n\033[1;37m%d\033[1;33m 2 типа;\n", statistic->out_amount[TYPE_1],
              statistic->out_amount[TYPE_2]);
       printf("\033[1;33mЗа это время машина:\nработала \033[1;37m%lf\033[1;33m секунд c 1 типом заявок;\nработала \033[1;37m%lf\033[1;33m секунд"
              " c 2 типом заявок;\nстояла \033[1;37m%lf\033[1;33m секунд;\n",
              statistic->work_time[TYPE_1],
              statistic->work_time[TYPE_2],
              statistic->waiting_time);
       printf("\033[1;33mСреднее время в очереди заявок 1 типа: \033[1;37m%lf\033[1;33m\nСреднее время в очереди заявок 2 типа: \033[1;37m%lf\n",
              statistic->all_queue_times[TYPE_1] / statistic->out_amount[TYPE_1],
              statistic->all_queue_times[TYPE_2] / statistic->out_amount[TYPE_2]);
       printf("\033[1;33mКоличество потерянных заявок (в связи с переполненной очередью): \033[1;37m%d\n",
              statistic->lost_apps);
}

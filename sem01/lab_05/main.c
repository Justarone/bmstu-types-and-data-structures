#include "main_cycles.h"
#include "array.h"
#include "additional_structs.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_FREE_ZONES 10000

int main()
{
    array_d free_zones = {};
    free_zones.data = (void **)malloc(MAX_FREE_ZONES * sizeof(void *));
    free_zones.mem_size = MAX_FREE_ZONES;

    stat_t statistic;
    statistic = list_cycle(&free_zones);
    // print something?

    statistic = array_cycle();
    printf("process time: %lf", statistic.process_time);
    free(free_zones.data);
}
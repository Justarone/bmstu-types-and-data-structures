#include "main_cycles.h"
#include "array.h"
#include "additional_structs.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_FREE_ZONES 10000

int main()
{
    array_d free_zones = {};
    free_zones.data = (void **)calloc(MAX_FREE_ZONES, sizeof(void *));
    free_zones.mem_size = MAX_FREE_ZONES;

    list_cycle(&free_zones);
    array_cycle();
    // free(free_zones.data);
}
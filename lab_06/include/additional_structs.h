#ifndef _ADDITIONAL_STRUCTS_H_
#define _ADDITIONAL_STRUCTS_H_

#include <stdint.h>

typedef struct stat_t
{
    int comp_num;
    uint64_t time;
    double avg_cmp;
} stat_t;

#endif
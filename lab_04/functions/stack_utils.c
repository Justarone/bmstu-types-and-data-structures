#include "stack_utils.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#define READED 1
#define MANUAL_MODE 0
#define RAND_MODE 1

#define READ_ERROR -11
#define ANSWER_ERROR -101
#define SCAN_ERROR -10

long long int scan_value(const int mode)
{
    if (mode != MANUAL_MODE && mode != RAND_MODE)
        return SCAN_ERROR;
    if (mode == RAND_MODE)
        return rand() % INT_MAX;

    long long int answer;

    if (scanf("%lli", &answer) != READED)
        return READ_ERROR;

    if (answer < 0 && answer >> 32)
        return ANSWER_ERROR;

    return answer;
}
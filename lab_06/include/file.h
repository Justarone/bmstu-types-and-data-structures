#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include "additional_structs.h"

int remove_f(FILE *const stream, const char *const key, stat_t *const stat);

#endif
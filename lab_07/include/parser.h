#ifndef _PARSER_H_
#define _PARSER_H_

#include "struct.h"
#include <stdio.h>

int avoid_cpy(node_t ***target, node_t **source, const int size, const int avnum);
int parser(node_t ***adj_list, int *const size, FILE *const stream);

#endif
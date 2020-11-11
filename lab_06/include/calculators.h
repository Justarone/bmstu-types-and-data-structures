#ifndef _CALCULATORS_H_
#define _CALCULATORS_H_

#include "additional_structs.h"
#include "hash.h"
#include "trees.h"

double count_cmp_tree(const node_t *const vertex);
double count_cmp_hash(const hash_t *const table);

#endif
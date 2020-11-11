#ifndef _IO_FUNCS_H_
#define _IO_FUNCS_H_

#include <stdio.h>
#include "hash.h"
#include "trees.h"
#include "additional_structs.h"

void print_menu();
int input_hash_table(hash_t **const table, FILE *const stream);
int input_tree(node_t **vertex_b, FILE *const stream);
void tree_to_atree(node_t *vertex_b, node_t **vertex);
void print_hash_table(const hash_t *const table);
void print_tree(const node_t *const vertex, const int depth);
void print_stat(const stat_t *const stat_array);

#endif
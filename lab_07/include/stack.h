#ifndef _STACK_H_
#define _STACK_H_

#include "struct.h"

node_t *push(const node_t *const head, const ntype value);
ntype pop(node_t **head);

#endif
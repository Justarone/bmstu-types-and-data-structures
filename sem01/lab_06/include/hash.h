#ifndef _HASH_H_
#define _HASH_H_

// you'd better choose the simple number for the size
#define TABLE_SIZE 113

// struct for closed hashing
// typedef struct hash_c
// {
//     char *table[TABLE_SIZE];
// } hash_c;

// struct of an element of closed hashing
typedef struct node_o
{
    char *data;
    struct node_o *next;
} node_o;

// struct for opened hashing
// typedef struct hash_o
// {
//     node_o *table[TABLE_SIZE];
// } hash_o;

// FUNCTIONS

int add_c(char **const table, const char *const key);
int get_c(char **const table, const char *const key);
int remove_c(char **const table, const char *const key);
int add_o(node_o **const table, const char *const key);
int get_o(node_o **const table, const char *const key);
int remove_o(node_o **const table, const char *const key);

#endif
#include "hash.h"
#include <string.h>
#include <stdlib.h>

#define OK 0
#define GET_ERROR -2
#define ARG_ERROR -1
#define ADD_ERROR 1
#define REMOVE_ERROR 2

// this function counts hash sum (value)
int hash_function(const char *const key)
{
    if (!key)
        return ARG_ERROR;
    int sum = 0, i = 0;
    while (key[i])
        sum += key[i++];
    return sum % TABLE_SIZE;
}

// add to opened hash table
int add_c(char **const table, const char *const key)
{
    if (!key)
        return ADD_ERROR;

    int pos = hash_function(key);
    // nt == number of a try
    int nt = 0;
    while (!table[(pos + nt * nt) % TABLE_SIZE] && nt <= TABLE_SIZE)
        nt++;
    if (nt > TABLE_SIZE)
        return ADD_ERROR;
    table[(pos + nt * nt) % TABLE_SIZE] = (char *)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(table[(pos + nt * nt) % TABLE_SIZE], key);
    return OK;
}

int get_c(char **const table, const char *const key)
{
    if (!key)
        return GET_ERROR;

    int nt = 0;
    int pos = hash_function(key);
    while (strcmp(table[(pos + nt * nt) % TABLE_SIZE], key) && nt <= TABLE_SIZE)
        nt++;
    if (nt > TABLE_SIZE)
        return GET_ERROR;
    return (pos + nt * nt) % TABLE_SIZE;
}

// remove from closed hash table
int remove_c(char **const table, const char *const key)
{
    if (!key)
        return REMOVE_ERROR;

    int pos = get_c(table, key);
    if (pos == GET_ERROR)
        return REMOVE_ERROR;
    free(table[pos]);
    table[pos] = NULL;
    return OK;
}

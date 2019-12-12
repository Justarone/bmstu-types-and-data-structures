#include "file.h"
#include <string.h>

#define BUF_SIZE 255
#define READED 1

#define OK 0
#define NO_WORD_ERROR 12

static char buffer[BUF_SIZE];

static int delete_word(FILE *const stream, const int pos, const int len)
{
    fseek(stream, pos, SEEK_SET);
    for (int i = 0; i < len; i++)
        fputc(' ', stream);
    return OK;
}

int remove_f(FILE *const stream, const char *const key, stat_t *const stat)
{
    int cur_pos = 0;
    while (fscanf(stream, "%s", buffer) == READED)
    {
        ++stat->comp_num;
        if (!strcmp(buffer, key))
            return delete_word(stream, cur_pos, ftell(stream) - cur_pos);
        cur_pos = ftell(stream);
    }
    return NO_WORD_ERROR;
}
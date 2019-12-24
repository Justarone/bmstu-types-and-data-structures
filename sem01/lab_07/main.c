#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "algorythm.h"
#include "struct.h"

#define OK 0
#define MAIN_ERROR 100
#define READED 1

static char buffer[255];

int main()
{
    setbuf(stdout, NULL);

    node_t **adj_list = NULL;
    int size = 0;

    printf("Введите файл, для считывания данных: ");
    if (scanf("%255s", buffer) != READED)
    {
        printf("Неверное имя файла");
        return MAIN_ERROR;
    }

    FILE *f = fopen(buffer, "r");
    if (!f)
    {
        printf("Невозможно открыть файл.");
        return MAIN_ERROR;
    }

    // parser works fine
    if (parser(&adj_list, &size, f) != OK)
    {
        printf("Ошибка чтения информации из файла");
        fclose(f);
        return MAIN_ERROR;
    }
    fclose(f);

    // выделим переменные под обратные ребра (в том числе их количество)
    // и начальную вершину
    ntype *mb_sols = NULL;
    int mb_sols_size = 0;
    int start = -1;
    for (int i = 0; i < size; i++)
        if (adj_list[i])
        {
            start = i;
            break;
        }

    if (start == -1)
    {
        printf("Ошибка, связанная со списками смежности.\n");
        return MAIN_ERROR;
    }

    if (dgs(adj_list, size, &mb_sols, &mb_sols_size, start) != OK)
    {
        printf("Невозможно преобразовать в дерево (лес).");
        return OK;
    }

    node_t **new_adj_list = NULL;
    if (!mb_sols)
    {
        printf("Граф уже дерево.\n");
        goto adj_list_free;
    }

    for (int i = 0; i < mb_sols_size; i++)
    {
        avoid_cpy(&new_adj_list, adj_list, size, mb_sols[i]);
        int start = -1;
        for (int i = 0; i < size; i++)
            if (new_adj_list[i])
            {
                start = i;
                break;
            }

        if (start == -1)
            printf("Ошибка, связанная со списками смежности (для вершины %d).\n",
                   mb_sols[i]);
        else
        {
            // if (simple_dgs(new_adj_list, size, start, mb_sols[i]))
            //     printf("Плохой вариант... (%d)\n",
            //            mb_sols[i]);
            // else
            if (!simple_dgs(new_adj_list, size, start, mb_sols[i]))
                printf("Можете убрать вершину #%d и получите дерево\n", mb_sols[i]);
        }

        node_t *elem, *tmp;
        for (int i = 0; i < size; i++)
        {
            elem = new_adj_list[i];
            while (elem)
            {
                tmp = elem;
                elem = elem->next;
                free(tmp);
            }
        }
        free(new_adj_list);
    }

    node_t *elem, *tmp;
    for (int i = 0; i < size; i++)
    {
        elem = adj_list[i];
        while (elem)
        {
            tmp = elem;
            elem = elem->next;
            free(tmp);
        }
    }
    free(mb_sols);
adj_list_free:
    free(adj_list);
    return OK;
}
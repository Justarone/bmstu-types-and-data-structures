#include "output_funcs.h"
#include <stdio.h>

#define LIST_STACK_PUSH 0
#define LIST_STACK_POP 1
#define ARRAY_STACK_PUSH 2
#define ARRAY_STACK_POP 3

extern int freed_zone;
extern int new_zone;

void print_menu()
{
    printf("\n\n\nМЕНЮ\nВведите действие (цифру), который хотите исполнить:\n"
           "0. Выход;\n"
           "1. Добавить элементы в стек, реализованный списком;\n"
           "2. Добавить элементы в стек, реализованный массивом;\n"
           "3. Удалить элементы из стека-списка;\n"
           "4. Удалить элементы из стека-массива;\n"
           "5. Вывести стек-список;\n"
           "6. Вывести стек-массив;\n"
           "7. Вывести массив свободных областей;\n"
           "8. Вывод общей статистики;\n"
           "9. Вывод времени вставки и удаления элементов.\n"
           "\nВвод: ");
}

void print_free_zones(const array_d *const free_zones)
{
    printf("Количество элементов = %d;\n", free_zones->data_size);
    for (int i = 0; i < free_zones->data_size; i++)
        printf("%d. %p\n", i + 1, free_zones->data[i]);
}

void print_stack_l(const node_t *ps)
{
    if (ps == NULL)
    {
        printf("Стек пуст.\n");
        return;
    }
    printf("Стек, хранящийся в виде списка:\n");
    int i = 1;
    while (ps)
    {
        printf("%d. %p\n", i++, ps->value);
        ps = ps->next;
    }
}

void print_stack_a(stack_a *ps)
{
    if (ps->data == NULL)
    {
        printf("Стек пуст.\n");
        return;
    }
    int len = ps->last - ps->data + 1;
    printf("Стек, хранящийся в виде массива:\n");
    for (int i = 0; i < len; i++)
        printf("%d. %p\n", i + 1, *(ps->data + i));
}

void print_stat()
{
    printf("Количество памяти, взятой по-новой: %d\n"
           "Количество памяти, взятой из уже использованной: %d",
           new_zone, freed_zone);
}

void print_times(const times_t *const array)
{
    int flag = 1;
    if (array[LIST_STACK_PUSH].time != 0)
    {
        flag = 0;
        printf("Время добавления в стек-список %d элементов: %ld\n\n",
               array[LIST_STACK_PUSH].amount, array[LIST_STACK_PUSH].time);
    }

    if (array[LIST_STACK_POP].time != 0)
    {
        flag = 0;
        printf("Время удаления из стека-списка %d элементов: %ld\n\n",
               array[LIST_STACK_POP].amount, array[LIST_STACK_POP].time);
    }

    if (array[ARRAY_STACK_PUSH].time != 0)
    {
        flag = 0;
        printf("Время добавления в стек-массив %d элементов: %ld\n\n",
               array[ARRAY_STACK_PUSH].amount, array[ARRAY_STACK_PUSH].time);
    }

    if (array[ARRAY_STACK_POP].time != 0)
    {
        flag = 0;
        printf("Время удаления из стека-массива %d элементов: %ld\n\n",
               array[ARRAY_STACK_POP].amount, array[ARRAY_STACK_POP].time);
    }

    if (flag)
        printf("Время еще не замерялось...\n");
}
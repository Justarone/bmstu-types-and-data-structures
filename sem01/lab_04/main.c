#include "stack.h"
#include "array.h"
#include "stack_cdio.h"
#include "output_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array_utils.h"

#define READED 1
#define NOT_IN -1

#define INIT_SIZE_FREE_ZONES 1000000
#define MAX_AMOUNT 1000

#define LIST_STACK_PUSH 0
#define LIST_STACK_POP 1
#define ARRAY_STACK_PUSH 2
#define ARRAY_STACK_POP 3

#define OK 0
#define READ_ERROR 15
#define ALLOCATION_ERROR 17
#define PUSH_ERROR 18

extern int freed_zone;
extern int new_zone;

int main(void)
{
    // init array with free zones
    array_d *free_zones = (array_d *)malloc(sizeof(array_d));
    if (!free_zones)
        return ALLOCATION_ERROR;
    free_zones->data = malloc(sizeof(void *) * INIT_SIZE_FREE_ZONES);
    if (!free_zones->data)
    {
        free(free_zones);
        return ALLOCATION_ERROR;
    }
    free_zones->mem_size = INIT_SIZE_FREE_ZONES;
    free_zones->data_size = 0;

    // init stacks:
    node_t *ps = NULL;
    stack_a as; // array stack**
    as.cur_index = 0;

    // init times array:
    times_t times_array[4] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};

    // init some `buffer` variables:
    int err_code = 0;
    int amount;
    int mode;
    int choice;

    print_menu();
    while (scanf("%d", &choice) != READED)
    {
        puts("Введено не число, попробуйте снова:");
        char c;
        while (scanf("%c", &c) == READED && c != '\n')
            ;
    }
    while (1)
    {
        printf("\n\n");
        switch (choice)
        {
        case 0:
            puts("Выход...");
            goto free_label;

        case 1:
            printf("Введите количество элементов (от 1 до 1000): ");
            if (scanf("%d", &amount) != READED)
            {
                puts("Не удалось считать количество элементов.\n");
                break;
            }
            if (amount <= 0 || amount > MAX_AMOUNT)
            {
                puts("Неверное значение количества элементов.\n");
                break;
            }
            node_t *temp = add_st_l(&ps, &amount, &times_array[LIST_STACK_PUSH].time);
            if (!temp)
            {
                times_array[LIST_STACK_PUSH].amount = amount;
                // данный участок кода проверяет добавленные элементы
                node_t *tmp = ps;
                int pos;
                for (int i = 0; i < amount; i++)
                {
                    if ((pos = is_in(free_zones, tmp)) != NOT_IN)
                    {
                        freed_zone++;
                        delete_element(free_zones, pos);
                    }
                    else
                        new_zone++;
                    tmp = tmp->next;
                }
                if (amount == 0)
                    times_array[LIST_STACK_PUSH].time = 0;
                printf("Кажется, стек переполнился..."
                       " (Количество добавленных элементов: %d)\n",
                       amount);
                break;
            }
            times_array[LIST_STACK_PUSH].amount = amount;
            node_t *tmp = temp;
            int pos;
            for (int i = 0; i < amount; i++)
            {
                if ((pos = is_in(free_zones, tmp)) != NOT_IN)
                {
                    freed_zone++;
                    delete_element(free_zones, pos);
                }
                else
                    new_zone++;
                tmp = tmp->next;
            }
            printf("Элементы добавлены.\n");
            ps = temp;
            break;

        case 2:
            printf("Введите количество элементов (от 1 до 1000): ");
            if (scanf("%d", &amount) != READED)
            {
                puts("Не удалось считать количество элементов.\n");
                break;
            }
            if (amount <= 0 || amount > MAX_AMOUNT)
            {
                puts("Неверное значение количества элементов.\n");
                break;
            }
            printf("Введите режим:\n1. Случайное заполнение.\n2."
                   " Ручное заполнение.\nВвод: ");
            if (scanf("%d", &mode) != READED)
            {
                puts("Не удалось считать режим.\n");
                break;
            }
            if (mode != 1 && mode != 2)
            {
                puts("Неверное значение режима.\n");
                break;
            }

            if ((err_code = add_st_a(&as, &amount, mode % 2, &times_array[ARRAY_STACK_PUSH].time)))
            {
                if (err_code == READ_ERROR)
                    printf("Возникла ошибка при вводе адреса...");
                else
                    printf("Кажется, стек переполнился...");
                printf(" (Количество добавленных элементов: %d)\n",
                       amount);
                times_array[ARRAY_STACK_PUSH].amount = amount;
                break;
            }
            times_array[ARRAY_STACK_PUSH].amount = amount;
            printf("Элементы добавлены");
            break;

        case 3:
            printf("Введите количество элементов, которое нужно удалить"
                   "\n(от 1 до 1000; примечание: при вводе числа, большего количества элементов в стеке\n"
                   "стек очистится полностью): ");
            if (scanf("%d", &amount) != READED)
            {
                puts("Не удалось считать количество элементов.\n");
                break;
            }
            if (amount <= 0 || amount > MAX_AMOUNT)
            {
                puts("Неверное значение количества элементов.\n");
                break;
            }
            if ((err_code = cleann_l(&ps, amount, &times_array[LIST_STACK_POP].time,
                                     free_zones)) != amount)
            {
                times_array[LIST_STACK_POP].amount = err_code;
                printf("Кажется, стек закончился..."
                       " (Количество очищенных элементов: %d)\n",
                       err_code);
                break;
            }
            times_array[LIST_STACK_POP].amount = amount;
            printf("Нужное количество элементов очищено!");
            break;

        case 4:
            printf("Введите количество элементов, которое нужно удалить"
                   "\n(от 1 до 1000; примечание: при вводе числа, большего количества элементов в стеке\n"
                   "стек очистится полностью): ");
            if (scanf("%d", &amount) != READED)
            {
                puts("Не удалось считать количество элементов.\n");
                break;
            }
            if (amount <= 0 || amount > MAX_AMOUNT)
            {
                puts("Неверное значение количества элементов.\n");
                break;
            }

            if ((err_code = cleann_a(&as, amount, &times_array[ARRAY_STACK_POP].time)) != amount)
            {
                times_array[ARRAY_STACK_POP].amount = err_code;
                if (err_code == 0)
                    times_array[ARRAY_STACK_POP].time = 0;
                printf("Кажется, стек закончился..."
                       " (Количество очищенных элементов: %d)\n",
                       err_code);
                break;
            }
            times_array[ARRAY_STACK_POP].amount = amount;
            printf("Нужное количество элементов очищено!");
            break;

        case 5:
            print_stack_l(ps);
            break;

        case 6:
            print_stack_a(&as);
            break;

        case 7:
            print_free_zones(free_zones);
            break;

        case 8:
            print_stat();
            break;

        case 9:
            print_times(times_array);
            break;

        default:
            printf("Введено число, не соответствующее ни одному из пунктов. Попробуйте снова.\n");
            break;
        }
        print_menu();
        while (scanf("%d", &choice) != READED)
        {
            puts("\n\nВведено не число, попробуйте снова:");
            char c;
            while (scanf("%c", &c) == READED && c != '\n')
                ;
        }
    }
free_label:
    free(free_zones->data);
    free(free_zones);
    while (ps)
    {
        node_t *temp = ps;
        ps = ps->next;
        free(temp);
    }
    return OK;
}
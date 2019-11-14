#include "stack.h"
#include "array.h"
#include "stack_cdio.h"
#include "output_funcs.h"
#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define READED 1
#define READ_ERROR 15

#define INIT_SIZE_FREE_ZONES 1000000
#define MAX_AMOUNT 1000

#define ALLOCATION_ERROR 17
#define PUSH_ERROR 18

int main(void)
{
    int choice;
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
    stack_a as = {NULL, NULL}; // array stack**

    int err_code = 0;
    int amount;
    int mode;

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
            node_t *temp = add_st_l(&ps, &amount, free_zones);
            if (!temp)
            {
                printf("Кажется, стек переполнился..."
                       " (Количество добавленных элементов: %d)\n",
                       amount);
                break;
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
            if ((err_code = add_st_a(&as, &amount, free_zones, mode % 2)))
            {
                if (err_code == READ_ERROR)
                    printf("Возникла ошибка при вводе адреса...");
                else
                    printf("Кажется, стек переполнился...");
                printf(" (Количество добавленных элементов: %d)\n",
                       amount);
                break;
            }
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
            if ((err_code = cleann_l(&ps, amount, free_zones)) != amount)
            {
                printf("Кажется, стек закончился..."
                       " (Количество очищенных элементов: %d)\n",
                       err_code);
                break;
            }
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
            if ((err_code = cleann_a(&as, amount, free_zones)) != amount)
            {
                printf("Кажется, стек закончился..."
                       " (Количество очищенных элементов: %d)\n",
                       err_code);
                break;
            }
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
    if (as.data)
    {
        free(as.data);
    }
    return OK;
}
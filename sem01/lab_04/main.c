#include "stack.h"
#include "array.h"
#include "stack_cdio.h"
#include "output_funcs.h"
#include <stdio.h>

#define OK 0
#define READED 1
#define INIT_SIZE_FREE_ZONES 1000000

#define ALLOCATION_ERROR 17

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

    // init stacks:
    node_t *ps = NULL;
    stack_a as = {NULL, NULL}; // array stack**

    int err_code = 0;
    int amount;

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
            return OK;

        case 1:
            printf("Введите количество элементов (от 1 до 10000): ");
            if (scanf("%d", &amount) != READED)
            {
                puts("Не удалось считать количество элементов.\n");
                break;
            }
            if (amount <= 0 || amount > 10000)
            {
                puts("Неверное значение количества элементов.\n");
                break;
            }
            add_st_l(ps, amount, free_zones);
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        case 6:
            break;

        case 7:
            break;

        case 8:
            break;

        case 9:
            break;

        default:
            printf("Введено число, не соответствующее ни одному из пунктов. Попробуйте снова.\n");
            break;
        }
        print_menu();
        while (scanf("%d", &choice) != READED)
        {
            puts("Введено не число, попробуйте снова:");
            char c;
            while (scanf("%c", &c) == READED && c != '\n')
                ;
        }
    }
    free(free_zones->data);
    free(free_zones);
    return OK;
}
// Проверить правильность ввода-вывода
// Отследить переполнение
// Наладить работу с чтением из файла

#include <stdio.h>
#include "headers/print_funcs.h"
#include "headers/student_struct.h"
#include "headers/struct_utils.h"
#include "headers/read.h"
#include <time.h>

#define OK 0
#define READED 1
#define KEYS 0
#define QUICK_SORT 1
#define BUBBLE_SORT 2
#define STUD_SIZE 150
#define FILENAME "my_students_listx3.txt"
#define READ_ERROR 101
#define STUD_LIST_ERROR 102
#define KEYS_SORT_ERROR 103
#define BUBBLE_SORT_ERROR 104
#define QUICK_SORT_ERROR 105
#define ADDITION_ERROR 106
#define KEYS_ERROR 107
#define COMPARE_ERROR 108
#define DELETE_ERROR 109


int main(void)
{
    int choice;
    int sign;
    int year;

    student_t stud_list[STUD_SIZE];
    int keys[STUD_SIZE];
    int err_code = 0;
    clock_t start, end;
    double times[3] = {-1, -1, -1}; // 0 - время сортировки ключей,
    // 1 - время сортировки таблицы пузырьком, 2 - время быстрой сортировки таблицы
    FILE *rfile;
    rfile = fopen(FILENAME, "r");
    int list_size = read_from_file(stud_list, STUD_SIZE, rfile);
    fclose(rfile);
    if (list_size <= 0)
        return STUD_LIST_ERROR;

    for (int i = 0; i < list_size; i++)
        keys[i] = i;

    print_menu();
    while (scanf("%d", &choice) != READED)
        puts("Введено не число, попробуйте снова:");
    while (1)
    {
        switch (choice)
        {
            case 0:
                puts("Выход...");
                return OK;
            case 1:
                print_all(stud_list, list_size);
                break;
            case 2:
                err_code = print_by_keys(stud_list, keys, list_size);
                if (err_code != OK)
                    return KEYS_ERROR;
                break;
            case 3:
                printf("Введите год поступления, студентов которого хотите вывести: ");
                if (scanf("%d", &year) != READED)
                {
                    puts("Не удалось считать год.");
                    break;
                }
                print_by_year(stud_list, list_size, year);
                break;
            case 4:
                sign = choose_property();
                if (sign == -1)
                {
                    puts("Введен некорректный номер признака сравнения!");
                    break;
                }
                start = clock();
                sort_by_keys(stud_list, keys, 0, list_size - 1, sign);
                end = clock();
                times[KEYS] = (double) (end - start) / CLOCKS_PER_SEC;
                break;
            case 5:
                sign = choose_property();
                if (sign == -1)
                {
                    puts("Введен некорректный номер признака сравнения!");
                    break;
                }
                start = clock();
                bubble_sort_list(stud_list, keys, list_size, sign);
                end = clock();
                times[BUBBLE_SORT] = (double) (end - start) / CLOCKS_PER_SEC;
                break;
            case 6:
                sign = choose_property();
                if (sign == -1)
                {
                    puts("Введен некорректный номер признака сравнения!");
                    break;
                }
                start = clock();
                quick_sort_list(stud_list, keys, list_size, 0, list_size - 1, sign);
                end = clock();
                times[QUICK_SORT] = (double) (end - start) / CLOCKS_PER_SEC;
                break;
            case 7:
                err_code = add_student(stud_list, &list_size, stdin);
                if (err_code != OK)
                    return ADDITION_ERROR;
                keys[list_size - 1] = list_size - 1;
                break;
            case 8:
                err_code = print_keys(keys, stud_list, list_size);
                if (err_code != OK)
                    return KEYS_ERROR;
                break;
            case 9:
                err_code = print_compare(times);
                if (err_code != OK)
                    puts("p.s.: как только будет использовано 2 вида сортировок из 3, данная опция будет доступна.");
                break;
            case 10:
                err_code = delete_student(stud_list, &list_size, keys);
                if (err_code != OK)
                    return DELETE_ERROR;
                break;
            case 11:
                rfile = fopen(FILENAME, "r");
                list_size = read_from_file(stud_list, STUD_SIZE, rfile);
                fclose(rfile);
                if (list_size <= 0)
                    return STUD_LIST_ERROR;
                for (int i = 0; i < list_size; i++)
                    keys[i] = i;
                break;
            default:
                puts("Ошибка: введено неверное число (попробуйте снова):\n");
                break;
        }
        print_menu();
        while (scanf("%d", &choice) != READED)
            puts("Введено не число, попробуйте снова:");
    }
    return OK;
}
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
#define QUICK_SORT 1
#define BUBBLE_SORT 2
#define QUICK_KEYS 0
#define BUBBLE_KEYS 3
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
    int err_code = 0;
    clock_t start, end;
    double times[4] = {-1, -1, -1, -1}; // 0 - время сортировки ключей,
    // 1 - время сортировки таблицы пузырьком, 2 - время быстрой сортировки таблицы
    FILE *rfile;
    int list_size = 0;
    keys_t keys_array[STUD_SIZE];

    print_menu();
    while (scanf("%d", &choice) != READED)
    {
        puts("Введено не число, попробуйте снова:");
        char c;
        while (scanf("%c", &c) == READED && c != '\n');
    }
    while (1)
    {
        switch (choice)
        {
            case 0:
                puts("Выход...");
                return OK;
                
            case 1:
                if (list_size == 0)
                {
                    puts("Список пустой. Печатать нечего.");
                    break;
                }
                print_all(stud_list, list_size);
                break;

            case 2:
                if (list_size == 0)
                {
                    puts("Список пустой. Печатать нечего.");
                    break;
                }
                print_keys_array(keys_array, list_size);
                break;

            case 3:
                if (list_size == 0)
                {
                    puts("Список пустой. Студентов нет");
                    break;
                }
                printf("Введите год поступления, студентов которого хотите вывести: ");
                if (scanf("%d", &year) != READED)
                {
                    puts("Не удалось считать год.");
                    break;
                }
                print_by_year(stud_list, list_size, year);
                break;

            case 4:
                if (list_size == 0)
                {
                    puts("Список пустой. Сортировать нечего.");
                    break;
                }
                start = clock();
                quick_sort_keys(keys_array, 0, list_size - 1);
                end = clock();
                times[QUICK_KEYS] = (double) (end - start) / CLOCKS_PER_SEC;
                break;

            case 5:
                if (list_size == 0)
                {
                    puts("Список пустой. Сортировать нечего.");
                    break;
                }
                start = clock();
                bubble_sort_keys(keys_array, list_size);
                end = clock();
                times[BUBBLE_KEYS] = (double) (end - start) / CLOCKS_PER_SEC;
                break;

            case 6:
                if (list_size == 0)
                {
                    puts("Список пустой. Сортировать нечего.");
                    break;
                }
                sign = choose_property();
                if (sign == -1)
                {
                    puts("Введен некорректный номер признака сравнения!");
                    break;
                }
                if (list_size == 0)
                {
                    puts("Список пустой. Сортировать нечего.");
                    break;
                }
                start = clock();
                bubble_sort_list(stud_list, list_size, sign);
                end = clock();
                times[BUBBLE_SORT] = (double) (end - start) / CLOCKS_PER_SEC;
                for (int i = 0; i < list_size; i++)
                {
                    keys_array[i].key = i;
                    keys_array[i].year = stud_list[i].adm_year;
                }
                break;

            case 7:
                if (list_size == 0)
                {
                    puts("Список пустой. Сортировать нечего.");
                    break;
                }
                sign = choose_property();
                if (sign == -1)
                {
                    puts("Введен некорректный номер признака сравнения!");
                    break;
                }
                start = clock();
                quick_sort_list(stud_list, 0, list_size - 1, sign);
                end = clock();
                times[QUICK_SORT] = (double) (end - start) / CLOCKS_PER_SEC;
                for (int i = 0; i < list_size; i++)
                {
                    keys_array[i].key = i;
                    keys_array[i].year = stud_list[i].adm_year;
                }
                break;

            case 8:
                err_code = add_student(stud_list, &list_size, stdin);
                if (err_code != OK)
                    return ADDITION_ERROR;
                keys_array[list_size - 1].key = list_size - 1;
                keys_array[list_size - 1].year = stud_list[list_size - 1].adm_year;
                break;

            case 9:
                err_code = print_compare(times);
                if (err_code != OK)
                    puts("p.s.: как только будет использовано 2 вида сортировок из 4, данная опция будет доступна.");
                break;

            case 10:
                if (list_size == 0)
                {
                    puts("Список пустой. Удалять некого.");
                    break;
                }
                err_code = delete_student(stud_list, &list_size);
                if (err_code != OK)
                    return DELETE_ERROR;
                for (int i = 0; i < list_size; i++)
                {
                    keys_array[i].key = i;
                    keys_array[i].year = stud_list[i].adm_year;
                }
                break;

            case 11:
                rfile = fopen(FILENAME, "r");
                list_size = read_from_file(stud_list, STUD_SIZE, rfile);
                fclose(rfile);
                if (list_size <= 0)
                    return STUD_LIST_ERROR;
                for (int i = 0; i < list_size; i++)
                {
                    keys_array[i].key = i;
                    keys_array[i].year = stud_list[i].adm_year;
                }
                break;

            case 12:
                if (list_size == 0)
                {
                    puts("Список пустой. Печатать нечего.");
                    break;
                }
                print_table_by_keys(keys_array, stud_list, list_size);
                break;
            
        }
        print_menu();
        while (scanf("%d", &choice) != READED)
        {
            puts("Введено не число, попробуйте снова:");
            char c;
            while (scanf("%c", &c) == READED && c != '\n');
        }
    }
    return OK;
}


// Нужно было много раз менять, поэтому было хорошее время
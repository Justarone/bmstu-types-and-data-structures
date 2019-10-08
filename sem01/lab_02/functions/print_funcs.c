#include "../headers/print_funcs.h"
#include <stdio.h>
#include <math.h>

#define SIZE_ERROR 11
#define READED 1
#define SET_PROPERTY_INFO_ERROR -1
#define OK 0
#define QUICK_SORT 1
#define BUBBLE_SORT 2
#define QUICK_KEYS 0
#define BUBBLE_KEYS 3
#define EPS 1e-4
#define TIMES_ERROR 12
#define HAT "|   |                                |                 |         |\
        |  год   |    тип    |                           | номер дома | квартира  |\n"\
"| № |             Фамилия            |       имя       |   пол   | оценка |\
  пост. |   жилья   |           улица           | (общежития)| (комната) |\n"\
"|---|--------------------------------|-----------------|---------|\
--------|--------|-----------|---------------------------|------------|-----------|\n"

int choose_property()
{
    printf(
"Таблица полей для сравнения:\n\
код поля | поле\n\
    1    | фамилия\n\
    2    | имя\n\
    3    | средний балл\n\
    4    | год поступления\n\
    5    | тип дома (дом считается \"бОльшим\")\n\
    6    | тип дома (общежитие считается \"бОльшим\")\n\
    7    | пол (женщина \"больше\")\n\
    8    | пол (мужчина \"больше\")\n\
\
Введите поле, по которому сортировать список (относительно этого поля будут сравниваться структуры): "
    );
    int answer;
    if ((scanf("%d", &answer)) != READED)
        return SET_PROPERTY_INFO_ERROR;
    if (answer < 1 || answer > 8)
        return SET_PROPERTY_INFO_ERROR;
    return answer;
}

void print_menu(void)
{
    printf(
        "\n\nМЕНЮ (Введите цифру, под которой находится желаемый вариант):\n"
        "0. Выход из программы;\n"
        "1. Вывод списка студентов группы;\n"
        "2. Вывод таблицы ключей;\n"
        "3. Вывод студентов, поступивших в определенном году и живущих в общежитии;\n"
        "4. Быстрая сортировка таблицы ключей;\n"
        "5. Сортировка таблицы ключей пузырьком;\n"
        "6. Сортировка таблицы пузырьком;\n"
        "7. Быстрая сортировка таблицы;\n"
        "8. Ввод нового студента;\n"
        "9. Вывод результатов сравнения алгоритмов сортировок;\n"
        "10. Удаление студента;\n"
        "11. Выгрузка списка студентов из файла;\n"
        "12. Вывод списка студента по ключам;\n\n"
        "Ввод: "
    );
}

void print_student(const student_t student, const int index)
{
    printf("|%3d| ", index);
    printf("%30s | %15s |", student.surname, student.name);
    printf((student.sex) ? " женский |" : " мужской |");
    printf(" %6.2f |", student.avg_score);
    printf(" %6u |", student.adm_year);
    if (student.home_type == HOUSE)
    {
        printf(
            "       дом |"
            " %25s | %10u | %9u |\n", 
            student.home.house.street, student.home.house.number, student.home.house.apt);  
    }
    else if (student.home_type == DORMITORY)
    {
        printf(
            " общежитие |"
            "      улица не указывается | %10u | %9u |\n", student.home.dorm.number, student.home.dorm.room
            );
    }
}


int print_compare(const double *const times)
{
    int flag = 1;
    if (fabs(times[QUICK_SORT] + 1.0) > EPS && fabs(times[BUBBLE_SORT] + 1.0) > EPS)
    {
        printf("Быстрая сортировка таблицы: %lf;\nСортировка таблицы пузырьком: %lf;\n\
разница = %lf%%\n", times[QUICK_SORT], \
        times[BUBBLE_SORT], (times[BUBBLE_SORT] - times[QUICK_SORT]) * 100 / times[BUBBLE_SORT]);
        flag = 0;
    }

    if (fabs(times[QUICK_SORT] + 1.0) > EPS && fabs(times[QUICK_KEYS] + 1.0) > EPS)
    {
        printf("Быстрая сортировка таблицы: %lf;\nБыстрая сортировка ключей: %lf;\n\
разница = %lf%%\n", times[QUICK_SORT], \
        times[QUICK_KEYS], (times[QUICK_SORT] - times[QUICK_KEYS]) * 100 / times[QUICK_SORT]);
        flag = 0;
    }

    if (fabs(times[BUBBLE_KEYS] + 1.0) > EPS && fabs(times[BUBBLE_SORT] + 1.0) > EPS)
    {
        printf("Сортировка ключей пузырьком: %lf;\nСортировка таблицы пузырьком: %lf;\n\
разница = %lf%%\n", times[BUBBLE_KEYS], \
        times[BUBBLE_SORT], (times[BUBBLE_SORT] - times[BUBBLE_KEYS]) * 100 / times[BUBBLE_SORT]);
        flag = 0;
    }

    if (fabs(times[BUBBLE_KEYS] + 1.0) > EPS && fabs(times[QUICK_KEYS] + 1.0) > EPS)
    {
        printf("Быстрая сортировка ключей: %lf;\nСортировка ключей пузырьком: %lf;\n\
разница = %lf%%\n", times[QUICK_KEYS], \
        times[BUBBLE_KEYS], (times[BUBBLE_KEYS] - times[QUICK_KEYS]) * 100 / times[BUBBLE_KEYS]);
        flag = 0;
    }

    if (flag)
    {
        puts("Для сравнения сортировок произведено недостаточно различных методов (минимум 2)!\n");
        return TIMES_ERROR;
    }
    else
        return OK;
}

void print_by_year(const student_t *const stud_list, const int size, const int year)
{
    int flag = 1;
    for (int i = 0; i < size; i++)
        if (stud_list[i].home_type == DORMITORY && stud_list[i].adm_year == year)
        {
            if (flag)
                printf(HAT);
            flag = 0;
            print_student(stud_list[i], i + 1);
        }
    if (flag)
        puts("Не найдены студенты, поступившие в этот год");
}

void print_all(const student_t *const stud_arr, const int size)
{
    if (size > 0)
        printf(HAT);
    else
        puts("В таблице нет студентов...");
    for (int i = 0; i < size; i++)
        print_student(stud_arr[i], i + 1);
}

void print_keys_array(const keys_t *const key_array, const int size)
{
    for (int i = 0; i < size; i++)
        printf("key: %d ----> year: %d\n", key_array[i].key + 1, key_array[i].year);
    puts("\n");
}

void print_table_by_keys(const keys_t *const key_array, const student_t *const stud_list, const int size)
{
    printf(HAT);
    for (int i = 0; i < size; i++)
        print_student(stud_list[key_array[i].key], key_array[i].key + 1);
}
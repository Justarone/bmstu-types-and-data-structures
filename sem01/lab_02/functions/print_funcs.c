#include "../headers/print_funcs.h"
#include <stdio.h>
#include <math.h>

#define SIZE_ERROR 11
#define READED 1
#define SET_PROPERTY_INFO_ERROR -1
#define OK 0
#define KEYS 0
#define QUICK_SORT 1
#define BUBBLE_SORT 2
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
        "2. Вывод таблицы в порядке ключей;\n"
        "3. Вывод студентов, поступивших в определенном году и живущих в общежитии;\n"
        "4. Сортировка массива ключей;\n"
        "5. Сортировка таблицы пузырьком;\n"
        "6. Быстрая сортировка таблицы;\n"
        "7. Ввод нового студента;\n"
        "8. Просмотр таблицы ключей;\n"
        "9. Вывод результатов сравнения алгоритмов сортировок;\n"
        "10. Удаление студента;\n"
        "11. Выгрузка списка студентов из файла заново;\n\n\n"
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


int print_by_keys(const student_t *const stud_list, const int *const keys, 
const int keys_size)
{
    if (keys_size <= 0)
        return SIZE_ERROR;
    for (int i = 0; i < keys_size; i++)
    {
        // printf("key: %d --> index in students table: %d\n\n", i + 1, keys[i] + 1);
        print_student(stud_list[keys[i]], keys[i] + 1);
    }
    return OK;
}

int print_keys(const int *const keys, const student_t *const stud_list, const int size)
{
    if (size <= 0)
        return SIZE_ERROR;
    for (int i = 0; i < size; i++)
        printf("key: %d --> index in student table: %d --> surname: %s\n\n", i + 1, keys[i] + 1, stud_list[keys[i]].surname);
    puts("\n\n");
    return OK;
}

int print_compare(const double *const times)
{
    if (fabs(times[KEYS] + 1.0) > EPS && fabs(times[QUICK_SORT] + 1.0) > EPS)
    {
        printf("keys quick sort: %lf;\ntable quick sort: %lf;\ndiff (keys vs quick sort table) = %lf", times[KEYS], \
        times[QUICK_SORT], (times[KEYS] - times[QUICK_SORT]) / times[KEYS]);
        return OK;
    }

    if (fabs(times[KEYS] + 1.0) > EPS && fabs(times[BUBBLE_SORT] + 1.0) > EPS)
    {
        printf("keys quick sort: %lf;\ntable bubble sort: %lf;\ndiff (keys vs bubble sort table) = %lf", times[KEYS], \
        times[BUBBLE_SORT], (times[KEYS] - times[BUBBLE_SORT]) / times[KEYS]);
        return OK;
    }

    if (fabs(times[QUICK_SORT] + 1.0) > EPS && fabs(times[BUBBLE_SORT] + 1.0) > EPS)
    {
        printf("table quick sort: %lf;\ntable bubble sort: %lf;\ndiff (quick vs bubble sorts of table) = %lf", times[QUICK_SORT], \
        times[BUBBLE_SORT], (times[QUICK_SORT] - times[BUBBLE_SORT]) / times[QUICK_SORT]);
        return OK;
    }

    puts("Для сравнения сортировок произведено недостаточно различных методов (минимум 2)!\n");
    return TIMES_ERROR;
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
    for (int i = 0; i < size; i++)
        print_student(stud_arr[i], i + 1);
}
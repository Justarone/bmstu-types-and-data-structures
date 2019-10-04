#include "../headers/struct_utils.h"
#include "../headers/print_funcs.h"
#include <stdio.h>
#include <ctype.h>

#define ADD_ERROR 201
#define EOF_ERROR 207
#define READED 1
#define EPS 1e-4
#define OK 0
#define TOO_BIG_LINE 202
#define INCORRECT_SYMBOL 203
#define YES 1
#define NO 0
#define SIGN_ERROR 204
#define LESS -1
#define EQUAL 0
#define MORE 1
#define NO_INDEX -1

static int is_bet(const char a)
{
    if ((a - 'A' >= 0 && a - 'Z' <= 0) || (a - 'a' >= 0 && a - 'z' <= 0) || a == '-')
        return YES;
    else 
        return NO;
}

static int get_id_index(const int index, const int *const keys, const int size)
{
    for (int i = 0; i < size; i++)
        if (keys[i] == index)
            return i;
    return NO_INDEX;
}

// function for my input
static int my_gets(char *const buf, const int buf_size, int (*check)(const char), FILE *const stream)
{
    char c;
    int i = 0;
    int correct_flag = 1;
    while ((c = getc(stream)) == '\n');
    while (c != '\n' && c != '\0' && c != EOF && i < buf_size)
    {
        if (check(c))
            buf[i++] = c;
        else
        {
            correct_flag = 0;
            break;
        }
        c = getc(stream);
    }
    if (c == EOF && i == 0)
        return EOF_ERROR;
    if (c != '\n' && c != '\0' && c != EOF)
    {
        while ((c = getc(stream)) != '\n' && c != '\0' && c != EOF);
        if (!correct_flag)
            return INCORRECT_SYMBOL;
        else
            return TOO_BIG_LINE;
    }
    buf[i] = '\0';
    return OK;
}

static int strcmp_t(const char* const str1, const char *const str2)
{
    int i;
    for (i = 0; tolower(str1[i]) == tolower(str2[i]) && str1[i]; i++);
    if (str1[i] > str2[i])
        return MORE;
    else if (str1[i] == str2[i])
        return EQUAL;
    else 
        return LESS;
}

int add_student(student_t *const stud_list, int *const size, FILE *const f)
{
    printf("Студент живет в общежитии (1) или в доме (2)? (Введите цифру): ");
    unsigned int answer;
    int sc;
    while ((sc = fscanf(f, "%u", &answer)) != READED || (answer != 1 && answer != 2))
    {
        if (sc != READED)
        {
            if (sc == EOF)
                return ADD_ERROR;
            printf("Не удалось считать число. Попробуйте еще раз: ");
        }
        else
            printf("Не удалось распознать 1 или 2 в вводе. Попробуйте еще раз: ");
    }
    stud_list[*size].home_type = answer & 1;

    int rc;
    printf("Введите фамилию студента: ");
    while ((rc = my_gets(stud_list[*size].surname, SURNAME_LEN, is_bet, f)) != OK)
    {   
        if (rc == EOF_ERROR)
            return rc;
        if (rc == TOO_BIG_LINE)
            printf("Введено слишком длинное значение фамилии. Попробуйте еще раз: ");
        else
            printf("В фамилии найден некорректный символ. Попробуйте еще раз: ");
    }

    printf("Введите имя студента: ");
    while ((rc = my_gets(stud_list[*size].name, NAME_LEN, is_bet, f)) != OK)
    { 
        if (rc == EOF_ERROR)
            return rc;
        if (rc == TOO_BIG_LINE)
            printf("Введено слишком длинное значение имени. Попробуйте еще раз: ");
        else
            printf("В имени найден некорректный символ. Попробуйте еще раз: ");
    }

    printf("Введите пол (Цифрой: 1 - женский, 2 - мужской): ");
    while ((sc = fscanf(f, "%u", &answer)) != READED || (answer != 1 && answer != 2))
    {
        
        if (sc != READED)
        {
            if (sc == EOF)
                return EOF_ERROR;
            printf("Не удалось считать число. Попробуйте еще раз: ");
        }
        else
            printf("Не удалось распознать 1 или 2 в вводе. Попробуйте еще раз: ");
    }
    stud_list[*size].sex = answer & 1;

    printf("Введите средний балл за сессию: ");
    do
    {
        while ((sc = fscanf(f, "%f", &stud_list[*size].avg_score)) != READED)
        {
            if (sc == EOF)
                return EOF_ERROR;
            printf("Не удалось считать число. Попробуйте еще раз: ");
        }
        if (stud_list[*size].avg_score - 1.0 >= EPS && stud_list[*size].avg_score - 5.0 <= EPS)
            break;
        else
            printf("Введеная средняя оценка не находится в пределах от 1 до 5. Попробуйте еще раз: ");
    } while (1);

    printf("Введите год поступления в университет (с 1830 по 2019): ");
    while ((sc = fscanf(f, "%u", &answer)) != READED || answer > 2019 || answer < 1830)
    {
        if (sc != READED)
        {
            if (sc == EOF)
                return EOF_ERROR;
            printf("Не удалось считать число. Попробуйте еще раз: ");
        }
        else
            printf("Неверно введен год. Попробуйте еще раз: ");
    }
    stud_list[*size].adm_year = answer;
    
    if (stud_list[*size].home_type == HOUSE)
    {
        printf("Введите название улицы, на которой живет студент: ");
        while ((rc = my_gets(stud_list[*size].home.house.street, NAME_LEN, is_bet, f)) != OK)
        {   
            if (rc == EOF_ERROR)
                return rc;
            if (rc == TOO_BIG_LINE)
                printf("Введено слишком длинное значение названия улицы. Попробуйте еще раз: ");
            else
                printf("В названии улицы найден некорректный символ. Попробуйте еще раз: ");
        }

        printf("Введите номер дома студента (<10000): ");
        while ((sc = fscanf(f, "%u", &answer)) != READED || answer > 10000)
        {
            if (sc != READED)
            {
                if (sc == EOF)
                    return EOF_ERROR;
                printf("Не удалось считать номер дома. Попробуйте еще раз: ");
            }
            else
                printf("Номер слишком большой. Попробуйте еще раз: ");
        }
        stud_list[*size].home.house.number = answer;
        
        printf("Введите номер квартиры студента (<1000): ");
        while ((sc = fscanf(f, "%u", &answer)) != READED || answer > 1000)
        {
            if (sc != READED)
            {
                if (sc == EOF)
                    return EOF_ERROR;
                printf("Не удалось считать номер квартиры. Попробуйте еще раз: ");
            }
            else
                printf("Номер слишком большой. Попробуйте еще раз: ");
        }
        stud_list[*size].home.house.apt = answer;
    }

    else
    {
        printf("Введите номер общежития студента (<1000): ");
        while ((sc = fscanf(f, "%u", &answer)) != READED || answer > 1000)
        {
            if (sc != READED)
            {
                if (sc == EOF)
                    return EOF_ERROR;
                printf("Не удалось считать номер общежития. Попробуйте еще раз: ");
            }
            else
                printf("Номер слишком большой. Попробуйте еще раз: ");
        }
        stud_list[*size].home.dorm.number = answer;
        
        printf("Введите номер комнаты студента (<1000): ");
        while ((sc = fscanf(f, "%u", &answer)) != READED || answer > 1000)
        {
            if (sc != READED)
            {
                if (sc == EOF)
                    return EOF_ERROR;
                printf("Не удалось считать номер комнаты. Попробуйте еще раз: ");
            }
            else
                printf("Номер слишком большой. Попробуйте еще раз: ");
        }
        stud_list[*size].home.dorm.room = answer;
    }
    (*size)++;
    puts("Запись добавлена!");
    return OK;
}

int delete_student(student_t *const stud_list, int *const size, int *const keys)
{
    printf("Как будем удалять: по ключу или индексу? (Введите цифру: 1 - ключ, 2 - индекс): ");
    unsigned int answer;
    int sc;

    while ((sc = scanf("%u", &answer)) != READED || (answer != 1 && answer != 2))
    {
        if (sc != READED)
            printf("Не удалось считать число. Попробуйте еще раз: ");
        else
            printf("Не удалось распознать 1 или 2 в вводе. Попробуйте еще раз: ");
    }

    if (answer == 1)
    {
        print_by_keys(stud_list, keys, *size);
        printf("Введите номер ключа, по которому хотите удалить: ");
        int key_i;
        while ((scanf("%d", &key_i)) != READED || key_i <= 0 || key_i > *size)
        {
            if (sc != READED)
                printf("Не удалось считать число. Попробуйте еще раз: ");
            else
                printf("Неверный номер ключа. Попробуйте еще раз: ");
        }

        key_i--;
        int index = keys[key_i];

        for (int i = index; i < *size - 1; i++)
            stud_list[i] = stud_list[i + 1];

        for (int i = key_i; i < *size - 1; i++)
            keys[i] = keys[i + 1];

        for (int i = 0; i < *size - 1; i++)
            if (keys[i] > index)
                keys[i]--;

        (*size)--;
        puts("Студент успешно убран из списка.\n");
    }
    
    else
    {
        print_all(stud_list, *size);
        puts("Введите индекс студента, которого хотите удалить: ");
        int index;
        
        while ((sc = scanf("%d", &index)) != READED || index <= 0 || index > *size)
        {
            if (sc != READED)
                printf("Не удалось считать число. Попробуйте еще раз: ");
            else
                printf("Неверный индекс. Попробуйте еще раз: ");
        }

        index--;
        int i;

        for (int i = index; i < *size - 1; i++)
            stud_list[i] = stud_list[i + 1];
        
        for (int i = 0; keys[i] != index; i++);
        for (; i < *size - 1; i++)
            keys[i] = keys[i + 1];

        (*size)--;
        puts("Cтудент успешно убран из списка!\n");
    }
    return OK;
}

// Function, which compares students by sign
// Signs table:
// sign code    |    sign
//     1        |   surname
//     2        |    name
//     3        | average score
//     4        | admission year
//     5        |  home_type (house is greater)
//     6        |  home_type (dorm is greater)
//     7        |  sex (female is greater)
//     8        |  sex (male is greater)
// returns -1, if first argument is less than second
// return 0, if arguments are equal
// return 1, if first argument is bigger than second 
static int student_cmp(const student_t std1, const student_t std2, const int sign)
{
    if (sign < 1 || sign > 8)
        return SIGN_ERROR;
    switch (sign)
    {
        case 1:
            return strcmp_t(std1.surname, std2.surname);
        case 2:
            return strcmp_t(std1.name, std2.name);
        case 3:
            if (std1.avg_score - std2.avg_score > EPS)
                return MORE;
            else if (std2.avg_score - std1.avg_score > EPS)
                return LESS;
            else
                return EQUAL;
        case 4:
            if (std1.adm_year > std2.adm_year)
                return MORE;
            else if (std1.adm_year < std2.adm_year)
                return LESS;
            else
                return EQUAL;
        case 5:
            return (std2.home_type - std1.home_type);
        case 6:
            return (std1.home_type - std2.home_type);
        case 7:
            return (std1.sex - std2.sex);
        case 8:
            return (std2.sex - std1.sex);
    }
    return OK;
}

static void int_swap(int *const a, int *const b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

static void student_swap(student_t *const std1, student_t *const std2)
{
    student_t buf = *std1;
    *std1 = *std2;
    *std2 = buf;
}

void quick_sort_list(student_t *const stud_list, int *const keys, const int size, const int begin, const int end, const int sign)
{
   if (begin < end)
    {
        int left = begin, right = end;
        student_t middle = stud_list[(left + right) / 2];
        do
        {
            for (; student_cmp(stud_list[left], middle, sign) == LESS; left++);
            for (; student_cmp(stud_list[right], middle, sign) == MORE; right--);
            if (left <= right)
            {   
                if (student_cmp(stud_list[left], stud_list[right], sign) == MORE)
                // Данное условие загромождает сортировку и добавляет время, но при этом делает сортировку устойчивой
                {
                    student_swap(&stud_list[left], &stud_list[right]);
                    int_swap(keys + get_id_index(left, keys, size), keys + get_id_index(right, keys, size));
                }
                left++;
                right--;
            }
        } while (left <= right);
        quick_sort_list(stud_list, keys, size, begin, right, sign);
        quick_sort_list(stud_list, keys, size, left, end, sign);
    }
}

void bubble_sort_list(student_t *const stud_list, int *const keys, const int size, const int sign)
{
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (student_cmp(stud_list[j], stud_list[j + 1], sign) == MORE)
            {
                student_swap(&stud_list[j], &stud_list[j + 1]);
                int_swap(keys + get_id_index(j, keys, size), keys + get_id_index(j + 1, keys, size));
            }
}

void sort_by_keys(const student_t *const stud_list, int *const keys, const int begin, const int end, const int sign)
{
    if (begin < end)
    {
        int left = begin, right = end;
        int middle = keys[(left + right) / 2];
        do
        {
            for (; student_cmp(stud_list[keys[left]], stud_list[middle], sign) == LESS; left++);
            for (; student_cmp(stud_list[keys[right]], stud_list[middle], sign) == MORE; right--);
            if (left <= right)
            {   
                if (student_cmp(stud_list[keys[left]], stud_list[keys[right]], sign) == MORE)
                // Данное условие загромождает сортировку и добавляет время, но при этом делает сортировку устойчивой
                    int_swap(keys + left, keys + right);
                left++;
                right--;
            }
        } while (left <= right);
        sort_by_keys(stud_list, keys, begin, right, sign);
        sort_by_keys(stud_list, keys, left, end, sign);
    }
}


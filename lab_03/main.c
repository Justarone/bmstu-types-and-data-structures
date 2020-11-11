#include "matrix_struct.h"
#include "matrix_cdio.h"
#include "multiplying.h"
#include "print_functions.h"

#include <stdio.h>
#include <time.h>

#define OK 0
#define READED 1
#define INPUT_ERROR 10
#define MULT_ERROR 11
#define VECTOR_INDEXES 3

#define CLASSIC 1
#define SPECIAL 0

const char filenames[6][40] = {"matrix_files/matrix50x50.txt",
                               "matrix_files/matrix400x400.txt",
                               "matrix_files/matrix1000x1000.txt",
                               "matrix_files/matrix50x1.txt",
                               "matrix_files/matrix400x1.txt",
                               "matrix_files/matrix1000x1.txt"};
const int sizes[3] = {50, 400, 1000};

int main(void)
{
    matrix_t matrix1 = {};
    matrix_std matrix2 = {};
    matrix_t factor1 = {};
    matrix_std factor2 = {};
    matrix_t res1 = {};
    matrix_std res2 = {};
    int is_mult_res = 0;

    clock_t times[8] = {-1, -1};
    clock_t start;
    int choice;        // variable for menu
    int answer;        // variable for user's answers
    int count;         // variable for elements amount in matrix
    FILE *f;           // descriptor variable for file usage
    int user_sizes[2]; // variable for user's size input

    print_menu(stdout);
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
            //=====================================================================================================================
        case 1:
            is_mult_res = 0;
            printf("Заполнение матрицы из файла."
                   "\nВыберите размер матрицы из предложенных "
                   "(введите цифру от 1 до 3: 1 - 50х50, 2 - 400x400, 3 - 1000x1000): ");
            if (scanf("%d", &answer) != READED || answer < 1 || answer > 3)
            {
                printf("Ошибка! Введено неверное значение. Выход в меню.\n\n");
                goto read_label;
            }
            count = sizes[answer - 1] * sizes[answer - 1];
            if (create_matrix_t(&matrix1, sizes[answer - 1], sizes[answer - 1]) ||
                create_matrix_std(&matrix2, sizes[answer - 1], sizes[answer - 1]))
            {
                printf("Проблемы с созданием матрицы. Выход в меню.\n\n");
                goto read_label;
            }

            f = fopen(filenames[answer - 1], "r");
            if (!f)
            {
                printf("Проблемы с открытием файла. Выход в меню.\n\n");
                goto read_label;
            }
            printf("\nВведите процент заполненности (от 1 до 100): ");
            if (scanf("%d", &answer) != READED || answer < 1 || answer > 100)
            {
                printf("Ошибка! Введено неверное значение. Выход в меню.\n\n");
                goto read_label;
            }

            count = count * answer / 100;
            if (input_matrix_t(&matrix1, f, count))
            {
                printf("Ошибка заполнения матрицы. Выход в меню.\n\n");
                goto read_label;
            }
            fclose(f);
            convert_matrix(&matrix1, &matrix2);
            printf("Матрица заполнена!\n\n");
            break;
            //=====================================================================================================================
        case 2:
            is_mult_res = 0;
            printf("Заполнение матрицы вручную."
                   "\nВведите размеры матрицы через пробел"
                   "(величина каждого размера от 0 до 1000): ");
            if (scanf("%d%d", user_sizes, user_sizes + 1) != READED * 2 || user_sizes[0] <= 0 ||
                user_sizes[0] > MAX_SIZE_MATRIX || user_sizes[1] > MAX_SIZE_MATRIX || user_sizes[1] <= 0)
            {
                printf("Размеры введены неверно, выход в меню.\n\n");
                goto read_label;
            }

            count = user_sizes[0] * user_sizes[1];
            if (create_matrix_t(&matrix1, user_sizes[0], user_sizes[1]))
            {
                printf("Проблемы с созданием матрицы. Выход в меню.\n\n");
                goto read_label;
            }

            printf("\nВведите количество элементов матрицы (от 1 до максимального кол-ва): ");
            if (scanf("%d", &count) != READED || count > user_sizes[0] * user_sizes[1] ||
                count <= 0)
            {
                printf("Ошибка! Введено неверное количество. Выход в меню.\n\n");
                goto read_label;
            }
            printf("Ввод матрицы выполняется тройками: \"ряд столбец значение\".\n"
                   "Введите %d троек (по тройке в строке (внимание, индексация столбцов и строк с 0!)): \n",
                   count);
            if (input_matrix_t(&matrix1, stdin, count))
            {
                printf("Ошибка заполнения матрицы. Выход в меню.\n\n");
                goto read_label;
            }
            convert_matrix(&matrix1, &matrix2);
            printf("Матрица заполнена!\n\n");
            break;
            //=====================================================================================================================
        case 3:
            is_mult_res = 0;
            printf("Заполнение вектора из файла."
                   "\nВыберите размер вектора из предложенных "
                   "(введите цифру от 1 до 3: 1 - 50х1, 2 - 400x1, 3 - 1000x1): ");
            if (scanf("%d", &answer) != READED || answer < 1 || answer > 3)
            {
                printf("Ошибка! Введено неверное значение. Выход в меню.\n\n");
                goto read_label;
            }
            count = sizes[answer - 1];
            if (create_matrix_t(&factor1, sizes[answer - 1], 1) ||
                create_matrix_std(&factor2, sizes[answer - 1], 1))
            {
                printf("Проблемы с созданием вектора. Выход в меню.\n\n");
                goto read_label;
            }

            f = fopen(filenames[VECTOR_INDEXES + answer - 1], "r");
            if (!f)
            {
                printf("Проблемы с открытием файла. Выход в меню.\n\n");
                goto read_label;
            }
            printf("\nВведите процент заполненности (от 1 до 100): ");
            if (scanf("%d", &answer) != READED || answer < 1 || answer > 100)
            {
                fclose(f);
                printf("Ошибка! Введено неверное значение. Выход в меню.\n\n");
                goto read_label;
            }

            count = count * answer / 100;
            if (input_matrix_t(&factor1, f, count))
            {
                fclose(f);
                printf("Ошибка заполнения вектора. Выход в меню.\n\n");
                goto read_label;
            }
            fclose(f);
            convert_matrix(&factor1, &factor2);
            printf("Вектор заполнен!\n\n");
            break;
            //=====================================================================================================================
        case 4:
            is_mult_res = 0;
            printf("Заполнение вектора вручную."
                   "\nВведите размер вектора "
                   "(от 0 до 1000): ");
            if (scanf("%d", user_sizes) != READED || user_sizes[0] <= 0 ||
                user_sizes[0] > MAX_SIZE_MATRIX)
            {
                printf("Размер введен неверно, выход в меню.\n\n");
                goto read_label;
            }

            count = user_sizes[0];
            if (create_matrix_t(&factor1, user_sizes[0], 1))
            {
                printf("Проблемы с созданием вектора. Выход в меню.\n\n");
                goto read_label;
            }

            printf("\nВведите количество элементов вектора (от 1 до максимального кол-ва): ");
            if (scanf("%d", &count) != READED || count > user_sizes[0] ||
                count <= 0)
            {
                printf("Ошибка! Введено неверное количество. Выход в меню.\n\n");
                goto read_label;
            }
            printf("Ввод вектора выполняется тройками: \"ряд столбец значение\".\n"
                   "Введите %d троек (по тройке в строке (внимание, индексация столбцов и строк с 0!)): \n",
                   count);
            if (input_matrix_t(&factor1, stdin, count))
            {
                printf("Ошибка заполнения вектора. Выход в меню.\n\n");
                goto read_label;
            }
            convert_matrix(&factor1, &factor2);
            printf("Вектор заполнен!\n\n");
            break;
            //=====================================================================================================================
        case 5:
            if (create_matrix_std(&res2, matrix2.rows, 1))
            {
                printf("Ошибка создания матрицы-результата. Убедитесь, что вы правильно ввели матрицу и вектор.\n");
                goto read_label;
            }
            start = clock();
            if (multiply_std(&matrix2, &factor2, &res2) != OK)
            {
                is_mult_res = 0;
                printf("Не удалось выполнить произведение (проблема с размерами матриц)");
                break;
            }
            is_mult_res = 1;
            times[CLASSIC] = clock() - start;
            break;
            //=====================================================================================================================
        case 6:
            if (create_matrix_t(&res1, matrix1.rows, 1))
            {
                printf("Ошибка создания матрицы-результата. Убедитесь, что вы правильно ввели матрицу и вектор.\n");
                goto read_label;
            }
            start = clock();
            if (multiply_t(&matrix1, &factor1, &res1) != OK)
            {
                is_mult_res = 0;
                printf("Не удалось выполнить произведение (проблема с размерами матриц)");
                break;
            }
            is_mult_res = 2;
            times[SPECIAL] = clock() - start;
            break;
            //=====================================================================================================================
        case 7:
            print_times(times);
            break;
            //=====================================================================================================================
        case 8:
            if (is_mult_res == 2)
            {
                convert_matrix(&matrix1, &matrix2);
                convert_matrix(&factor1, &factor2);
                convert_matrix(&res1, &res2);
                if (print_elems(&matrix2, &factor2, &res2))
                {
                    printf("Печать в координатном виде...\n\n");
                    print_coords(&matrix2, &factor2, &res2);
                }
            }
            else if (is_mult_res == 1)
            {
                if (print_elems(&matrix2, &factor2, &res2))
                {
                    printf("Печать в координатном виде...\n\n");
                    print_coords(&matrix2, &factor2, &res2);
                }
            }
            else
                printf("Еще не было умножения.");
            break;

        default:
            printf("Введено некорректное значение, повторите.");
        }
    read_label:
        print_menu(stdout);
        while (scanf("%d", &choice) != READED)
        {
            puts("Введено не число, попробуйте снова:");
            char c;
            while (scanf("%c", &c) == READED && c != '\n')
                ;
        }
    }
    return OK;
}

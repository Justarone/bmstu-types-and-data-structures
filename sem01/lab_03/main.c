#include "matrix_struct.h"
#include "matrix_cdio.h"
#include "multiplying.h"
#include "print_functions.h"
#include <stdio.h>
#include <time.h>

#define OK 0
#define READED 1
#define INPUT_ERROR 10
#define VECTOR_INDEXES 3

#define CLASSIC 1
#define SPECIAL 0

const char filenames[6][30] = {"matrix_files/matrix50x50",
                               "matrix_files/matrix400x400",
                               "matrix_files/matrix1000x1000",
                               "matrix_files/matrix50x1",
                               "matrix_files/matrix400x1",
                               "matrix_files/matrix1000x1"};
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
                   "\nВыберете размер матрицы из предложенных "
                   "(введите цифру от 1 до 3: 1 - 50х50, 2 - 400x400, 3 - 1000x1000): ");
            if (scanf("%d", &answer) != READED || answer < 1 || answer > 3)
            {
                printf("Ошибка! Введено неверное значение. Завершение программы.");
                return INPUT_ERROR;
            }
            count = sizes[answer - 1] * sizes[answer - 1];
            if (create_matrix_t(&matrix1, sizes[answer - 1], sizes[answer - 1]) ||
                create_matrix_std(&matrix2, sizes[answer - 1], sizes[answer - 1]))
            {
                printf("Проблемы с созданием матрицы. Выход из программы.");
                return INPUT_ERROR;
            }

            f = fopen(filenames[answer - 1], "r");
            if (!f)
            {
                printf("Проблемы с открытием файла. Завершение программы.");
                return INPUT_ERROR;
            }
            printf("\nВведите процент заполненности (от 1 до 100): ");
            if (scanf("%d", answer) != READED || answer < 1 || answer > 100)
            {
                printf("Ошибка! Введено неверное значение. Завершение программы.");
                return INPUT_ERROR;
            }

            count = count * answer / 100;
            if (input_matrix_t(&matrix1, f, count))
            {
                printf("Ошибка заполнения матрицы. Выход из программы.");
                return INPUT_ERROR;
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
                printf("Размеры введены неверно, выход из программы.");
                return INPUT_ERROR;
            }

            count = user_sizes[0] * user_sizes[1];
            if (create_matrix_t(&matrix1, user_sizes[0], user_sizes[1]))
            {
                printf("Проблемы с созданием матрицы. Выход из программы.");
                return INPUT_ERROR;
            }

            printf("\nВведите количество элементов матрицы (от 1 до максимального кол-ва): ");
            if (scanf("%d", &count) != READED || count > user_sizes[0] * user_sizes[1] ||
                count <= 0)
            {
                printf("Ошибка! Введено неверное количество. Завершение программы.");
                return INPUT_ERROR;
            }
            printf("Ввод матрицы выполняется тройками: \"ряд столбец значение\".\n"
                   "Введите %d троек (по тройке в строке (внимание, индексация столбцов и строк с 0!)): \n",
                   count);
            if (input_matrix_t(&matrix1, stdin, count))
            {
                printf("Ошибка заполнения матрицы. Выход из программы.");
                return INPUT_ERROR;
            }
            printf("Матрица заполнена!\n\n");
            break;
            //=====================================================================================================================
        case 3:
            is_mult_res = 0;
            printf("Заполнение вектора из файла."
                   "\nВыберете размер вектора из предложенных "
                   "(введите цифру от 1 до 3: 1 - 50х1, 2 - 400x1, 3 - 1000x1): ");
            if (scanf("%d", &answer) != READED || answer < 1 || answer > 3)
            {
                printf("Ошибка! Введено неверное значение. Завершение программы.");
                return INPUT_ERROR;
            }
            count = sizes[answer - 1];
            if (create_matrix_t(&factor1, sizes[answer - 1], 1) ||
                create_matrix_std(&factor2, sizes[answer - 1], 1))
            {
                printf("Проблемы с созданием вектора. Выход из программы.");
                return INPUT_ERROR;
            }

            f = fopen(filenames[VECTOR_INDEXES + answer - 1], "r");
            if (!f)
            {
                printf("Проблемы с открытием файла. Завершение программы.");
                return INPUT_ERROR;
            }
            printf("\nВведите процент заполненности (от 1 до 100): ");
            if (scanf("%d", answer) != READED || answer < 1 || answer > 100)
            {
                fclose(f);
                printf("Ошибка! Введено неверное значение. Завершение программы.");
                return INPUT_ERROR;
            }

            count = count * answer / 100;
            if (input_matrix_t(&factor1, f, count))
            {
                fclose(f);
                printf("Ошибка заполнения вектора. Выход из программы.");
                return INPUT_ERROR;
            }
            fclose(f);
            convert_matrix(&matrix1, &matrix2);
            printf("Вектор заполнен!\n\n");
            break;
            //=====================================================================================================================
        case 4:
            is_mult_res = 0;
            printf("Заполнение вектора вручную."
                   "\nВведите размеры вектора"
                   "(от 0 до 1000): ");
            if (scanf("%d%d", user_sizes) != READED || user_sizes[0] <= 0 ||
                user_sizes[0] > MAX_SIZE_MATRIX)
            {
                printf("Размер введен неверно, выход из программы.");
                return INPUT_ERROR;
            }

            count = user_sizes[0];
            if (create_matrix_t(&matrix1, user_sizes[0], 1))
            {
                printf("Проблемы с созданием вектора. Выход из программы.");
                return INPUT_ERROR;
            }

            printf("\nВведите количество элементов вектора (от 1 до максимального кол-ва): ");
            if (scanf("%d", &count) != READED || count > user_sizes[0] * user_sizes[1] ||
                count <= 0)
            {
                printf("Ошибка! Введено неверное количество. Завершение программы.");
                return INPUT_ERROR;
            }
            printf("Ввод вектора выполняется тройками: \"ряд столбец значение\".\n"
                   "Введите %d троек (по тройке в строке (внимание, индексация столбцов и строк с 0!)): \n",
                   count);
            if (input_matrix_t(&matrix1, stdin, count))
            {
                printf("Ошибка заполнения вектора. Выход из программы.");
                return INPUT_ERROR;
            }
            printf("Вектор заполнен!\n\n");
            //=====================================================================================================================
        case 5:
            start = clock();
            if (multiply_std(&matrix2, &factor2, &res2) != OK)
            {
                is_mult_res = 0;
                printf("Не удалось выполнить произведение (проблема с размерами матриц)");
                break;
            }
            is_mult_res = 1;
            times[CLASSIC] = clock() - start;
            //=====================================================================================================================
        case 6:
            start = clock();
            if (multiply_t(&matrix1, &factor1, &res1) != OK)
            {
                is_mult_res = 0;
                printf("Не удалось выполнить произведение (проблема с размерами матриц)");
                break;
            }
            is_mult_res = 2;
            times[SPECIAL] = clock() - start;
            //=====================================================================================================================
        case 7:
            print_time(times);
            //=====================================================================================================================
        case 8:
            if (is_mult_res == 2)

            default:
                printf("Введено некорректное значение, повторите.");
        }
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

return 0;
}
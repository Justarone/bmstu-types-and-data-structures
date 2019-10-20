#include "matrix_struct.h"
#include "matrix_cdio.h"
#include "multiplying.h"
#include "print_functions.h"
#include <stdio.h>

#define OK 0
#define READED 1
#define INPUT_ERROR 10
#define VECTOR_INDEXES 3

const char filenames[6][30] = {"matrix50x50", "matrix400x400", "matrix1000x1000", "matrix50x1", "matrix400x1", "matrix1000x1"};
const int sizes[3] = {50, 400, 1000};

int main(void)
{
    matrix_t matrix1 = {};   // матрица, хранящаяся в особом виде
    matrix_std matrix2 = {}; // матрица, хранящаяся в стандартном виде
    matrix_t factor1 = {};   // вектор в особом виде
    matrix_std factor2 = {}; // вектор в стандартном виде
    matrix_t res1 = {};      // вектор-результат в особом виде
    matrix_std res2 = {};    // вектор-результат в стандартном виде

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
            //=====================================================================================================================
        case 2:
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
                   "Введите %d троек (по тройке в строке): \n",
                   count);
            if (input_matrix_t(&matrix1, stdin, count))
            {
                printf("Ошибка заполнения матрицы. Выход из программы.");
                return INPUT_ERROR;
            }
            printf("Матрица заполнена!\n\n");
            //=====================================================================================================================
        case 3:
            printf("Заполнение вектора из файла."
                   "\nВыберете размер вектора из предложенных "
                   "(введите цифру от 1 до 3: 1 - 50х1, 2 - 400x1, 3 - 1000x1): ");
            if (scanf("%d", &answer) != READED || answer < 1 || answer > 3)
            {
                printf("Ошибка! Введено неверное значение. Завершение программы.");
                return INPUT_ERROR;
            }
            count = sizes[answer - 1];
            if (create_matrix_t(&matrix1, sizes[answer - 1], 1) ||
                create_matrix_std(&matrix2, sizes[answer - 1], 1))
            {
                printf("Проблемы с созданием матрицы. Выход из программы.");
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
            if (input_matrix_t(&matrix1, f, count))
            {
                fclose(f);
                printf("Ошибка заполнения матрицы. Выход из программы.");
                return INPUT_ERROR;
            }
            fclose(f);
            convert_matrix(&matrix1, &matrix2);
            printf("Матрица заполнена!\n\n");

            //=====================================================================================================================
        case 4:
            //=====================================================================================================================
        case 5:
            //=====================================================================================================================
        case 6:
            //=====================================================================================================================
        case 7:
            //=====================================================================================================================
        case 8:

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
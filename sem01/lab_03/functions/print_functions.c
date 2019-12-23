#include "print_functions.h"
#include <time.h>

#define CLASSIC 1
#define SPECIAL 0

#define MENU_TEXT "\n\nМЕНЮ (выберете один пункт, введите цифру):\n"                              \
                  "0. Выход из программы;\n"                                                                       \
                  "1. Заполнение матрицы из файла;\n"                                                      \
                  "2. Заполнение матрицы вручную;\n"                                                       \
                  "3. Заполнение вектора из файла;\n"                                                      \
                  "4. Заполнение вектора вручную;\n"                                                       \
                  "5. Произведение классической матрицы на вектор;\n"                       \
                  "6. Произведение матрицы особого способа хранения на вектор;\n" \
                  "7. Вывод сравнения времени произведения;\n"                                    \
                  "8. Вывод результатов перемножения.\n\nВвод:"

void print_menu(FILE *const stream)
{
    fprintf(stream, MENU_TEXT);
}

// функция печати элементов (печати в обычном виде, только если каждый размер <= 10)
// печать происходит таким образом: ряд матрицы       элемент вектора-столбца        элемент вектора-результата (тоже столбца)
int print_elems(const matrix_std *const matrix1, const matrix_std *const matrix2, const matrix_std *const res)
{
    if (matrix1->columns > 10 || matrix1->rows > 10)
    {
        printf("Матрица слишком большая. Невозможно вывести ее целиком.\n");
        return 1;
    }

    for (int i = 0; i < matrix1->rows; i++)
    {
        for (int j = 0; j < matrix1->columns; j++)
            printf("%3d ", matrix1->data[i][j]);
        if (matrix2->rows > i)
            printf("   %3d    ", matrix2->data[i][0]);
        else
            printf("          ");
        printf("%3d\n", res->data[i][0]);
    }
    for (int i = matrix1->rows; i < matrix2->rows; i++)
    {
        for (int j = 0; j < matrix1->columns; j++)
            printf("    ");
        printf("   %3d    ", matrix2->data[i][0]);
        printf("\n");
    }
    printf("\n\n");
    return 0;
}

void print_times(const clock_t *const times)
{
    int flag = 1;
    if (times[SPECIAL] != -1)
    {
        flag = 0;
        printf("Время перемножения специальных матриц: %lf\n",
               (double)times[0] / CLOCKS_PER_SEC);
    }

    if (times[CLASSIC] != -1)
    {
        flag = 0;
        printf("Время классического перемножения: %lf\n",
               (double)times[CLASSIC] / CLOCKS_PER_SEC);
    }

    if (times[CLASSIC] != -1 && times[SPECIAL] != -1)
    {
        flag = 0;
        printf("Разница между методами: %lf%%",
               (double)100 * (times[CLASSIC] - times[SPECIAL]) / times[CLASSIC]);
    }

    if (flag)
        printf("Еще не было произведено ни одной корректной операции умножения.");
}

void print_coords(const matrix_std *const matrix, const matrix_std *const factor,
                  const matrix_std *const res)
{
    printf("\n\nmatrix:\n");
    printf(" i   j   value\n\n");
    for (int i = 0; i < matrix->rows; i++)
        for (int j = 0; j < matrix->columns; j++)
            if (matrix->data[i][j] != 0)
                printf("%3d %3d %5d\n", i, j, matrix->data[i][j]);

    printf("\n\nfactor matrix-column:\n");
    printf(" i   j   value\n\n");
    for (int i = 0; i < factor->rows; i++)
        for (int j = 0; j < factor->columns; j++)
            if (factor->data[i][j] != 0)
                printf("%3d %3d %5d\n", i, j, factor->data[i][j]);

    printf("\n\nresult matrix-row:\n");
    printf(" i   j   value\n\n");
    for (int i = 0; i < res->rows; i++)
        for (int j = 0; j < res->columns; j++)
            if (res->data[i][j] != 0)
                printf("%3d %3d %5d\n", i, j, res->data[i][j]);
}
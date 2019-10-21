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
                  "8. Вывод результатов перемножения.\n\n"

void print_menu(FILE *const stream)
{
    fprintf(stream, MENU_TEXT);
}

void print_elems(const matrix_std *const matrix1, const matrix_std *const matrix2, const matrix_std *const res)
{
    for (int i = 0; i < matrix1->rows; i++)
    {
        for (int j = 0; j < matrix1->columns; j++)
            printf("%3d ", matrix1->data[i][j]);
        printf("   %3d    ", matrix2->data[i][0]);
        printf("%3d\n", res->data[i][0]);
    }
    printf("\n\n");
}

void print_times(const clock_t *const times)
{
    if (times[SPECIAL] != -1)
        printf("Время перемножения специальных матриц: %lf\n",
               (double)times[0] / CLOCKS_PER_SEC);

    if (times[CLASSIC] != -1)
        printf("Время классического перемножения: %lf\n",
               (double)times[CLASSIC] / CLOCKS_PER_SEC);

    if (times[CLASSIC] != -1 && times[SPECIAL] != -1)
        printf("Разница между методами: %lf%%",
               (double)100 * (times[CLASSIC] - times[SPECIAL]) / times[CLASSIC]);
}
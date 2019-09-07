#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 40
#define OK 0
#define DIV_ERROR 1
#define CHECK_ERROR 2
#define YES 1
#define NO 0
#define MINUS 1
#define PLUS 0
#define RAW_DATA_LEN 2 // количество поступающих "сырых" данных
#define MANT_SIZE 32 // размер массива для хранения мантиссы (с учетом места под точку и '\0')
#define MANT_LEN 30 // длина мантиссы
#define EXP_SIZE 18
#define OPERANDS 3

typedef char raw_data_arr[RAW_DATA_LEN][MAXLEN + 1]; // массив для сырых данных, то есть для введенных пользователем строк
typedef char num30[MANT_SIZE]; // Тип "строка" для мантиссы
typedef struct // структура для хранения больших чисел
{
	short int sign_m : 1; // знак мантиссы
	num30 mant; // значение мантиссы
	int exp_num : EXP_SIZE; // порядок числа (значение после Е)
	unsigned int is_bad : 1; // бит истинен, если число испортилось в следствие деления (например, не лежит в заданном диапазоне или было деление на нуль)
} big_num;
typedef big_num big_nums_arr[OPERANDS]; 


int my_gets(char *const str, const int max_len); // Функция ввода данных 
int is_digit(const char c); // Функция проверки, является ли символ числом
int check_data(char *const str); // Функция проверки корректности ввода
int parse_raw_data(char *str, big_num *data); // Функция приведения строки к виду структуры хранения больших чисел
int mant_more_eq(const char *const mant1, const char *const mant2); // Функция ">=" для мантисс больших чисел
int mant_diff(char *const mant1, const char *const mant2); // Функция вычитания мантисс больших чисел
void move_mant(big_num *var, const int k); // Функция, выполняющая сдвиг мантиссы
int is_zero(const char *const var); // Функция проверки, является ли мантисса числа чистым нулем
big_num big_division(big_num a, big_num b); // Функция деления больших чисел
void remove_zeros(char *const str); // Функция, убирающая незначащие нули (пример: 0.234324000000 ==> 0.234324)
void big_print(big_num var); // Функция печати требуемых чисел
void pretty_print(const big_num *const data); // Функция печати данных в нужном виде


int main(void)
{
	setbuf(stdout, NULL); // отключение буферизации

	raw_data_arr str_nums; 
	big_nums_arr data;

	// Получаем данные от пользователя и сразу проверяем корректность
	printf("Введите число (делимое) в формате с точкой (.00025, +123001, –123.456) или \
экспоненциальном формате (1234567Е–20, 1234567Е20 или -123.4567Е23) и нажмите Enter:\n");
	my_gets(str_nums[0], MAXLEN);
	while (check_data(str_nums[0]))
	{
		printf("Неверный формат, повторите:\n");
		my_gets(str_nums[0], MAXLEN);
	}

	printf("Введите второе число (делитель) в том же формате и нажмите Enter:\n");
	my_gets(str_nums[1], MAXLEN);
	while (check_data(str_nums[1]))
	{
		printf("Неверный формат, повторите:\n");
		my_gets(str_nums[1], MAXLEN);
	}
	// Приводим данные к нужной структуре
	for (int i = 0; i < 2; i++)
		parse_raw_data(str_nums[i], &data[i]);
		// Выполняем деление и выводим результат
	data[2] = big_division(data[0], data[1]);
	if (!data[2].is_bad)
		pretty_print(data);
	else
	{
		printf("Ошибка. В результате деления получено некорректное число или было произведено деление на ноль.\n");
		return DIV_ERROR;
	}
	return OK;
}


int my_gets(char *const str, const int max_len) // 2ой параметр - ограничение на длину строки
{
	int k = 0;
	char c;
	while ((c = getchar()) == ' '); // пропускаем пробелы при вводе пользователя (игнорируем)
	while (k < max_len && c != '\n' && c != EOF && c != '\0') // пока не конец, не Enter и не достигнуто ограничение
	{
		str[k++] = c;
		c = getchar();
	}
	str[k] = '\0';
	if (k == max_len) // если достигнут лимит длины строки, то до конца строки (файла) пропускаем остальные символы
		// (также, после этого if'a можно вернуть ошибку, поэтому данная функция все-таки возвращает целое число в качестве кода ошибки)
		while ((c = getchar()) != '\n' && c != EOF && c != '\0');
	return OK;
}

int is_digit(const char c)
{
	return (c - '0' >= 0 && c - '9' <= 0) ? YES : NO;
}

int check_data(char *const str)
{
	int stage = 1; // стадии чтения составного числа (формально вот: +_23_._2343_Е_-_2343 - в данном случае стадии чтения ограничены символом "_")
	int point = 0; // переменная-флаг, указывающая была ли точка в мантиссе или нет (1 - была, 0 - нет)
	int e_counter = 0; // счетчик количества цифр в порядке числа (очевидно, что если их больше 6, то у нас переполнение)
	for (int i = 0; str[i]; i++)
	{
		switch (stage) 
		{
			case 1:
				if (str[i] == '+' || str[i] == '-' || is_digit(str[i])) // прочитан знак, следующая цифра (или точка)
					stage = 2;
				else if (str[i] == '.') // прочитана точка, пропускаем первую стадию с цифрой
					stage = 3;
				else if (str[i] == 'E') // мантиссы не ожидается, переход к стадии ввода знака и значения порядка
					stage = 5;
				else
					return CHECK_ERROR; // *ни один сценарий не случился*
				break;

			case 2:
				if (str[i] == 'E')
				{
					if (i > 30) // проверка на длину мантиссы
						return CHECK_ERROR;
					stage = 5;
				}
				else if (str[i] == '.')
					stage = 3;
				else if (!is_digit(str[i]))
					return CHECK_ERROR;
				break;

			case 3: // стадия "после точки". ожидается число. Если число получено, то переход к стадии ожидания "Е"
				point = 1; // фиксируем факт получения точки
				if (!is_digit(str[i]))
					return CHECK_ERROR;
				stage = 4;
				break;

			case 4: // стадия ожидания "Е"
				if (str[i] == 'E') 
				{
					if (i > 30 + point) // проверка длины мантиссы с учетом точки
						return CHECK_ERROR;
					stage = 5;
				}
				else if (!is_digit(str[i]))
					return CHECK_ERROR;
				break;

			case 5: // стадия ожидания знака или перехода к проверке значения "Е"
				if (str[i] == '+' || str[i] == '-' || (e_counter = is_digit(str[i])))
					stage = 6;
				else
					return CHECK_ERROR;
				break;

			case 6: // стадия окончательной проверки "Е"
				if (!is_digit(str[i]))
					return CHECK_ERROR;
				else
					e_counter++;
				if (e_counter == 6)
					return CHECK_ERROR;
				break;

			default:
				return CHECK_ERROR;
		}
	}
	return OK;
} 

int parse_raw_data(char *str, big_num *data)
{
	// работа с мантиссой
	int i = 0, j = 0;
	int inc = 0; // Требуется для обработки ситуации, когда вводится одна "Е"
	int point = 0; 
	data->exp_num = 0;
	data->sign_m = (str[i] == '-') ? MINUS : PLUS; // знак мантиссы (если есть). Записываем в отведенное поле структуры
	if (str[i] == '+' || str[i] == '-') // знак учтен, пропускаем, переходя к значению
		i++;
	else if (str[i] == 'E') // если знака и значения не было, а была "Е", то наше значение - 1
	{
		strcpy(data->mant, ".1000000000000000000000000000000\0");
		inc = 1;
	}
	while (is_digit(str[i]) || str[i] == '.') // теперь записываем значения
	{
		if (str[i] == '.') // проверяем, есть ли в числе точка
			point = 1;
		data->mant[j++] = str[i++];
	}
	// если в числе нет точки, то вставляем ее (после прочитанной целой части)
	// * это требуется для унификации процедуры выравнивания мантисс чисел
	if (!point)
		data->mant[j++] = '.'; 
	while (j < MANT_SIZE - 1) // дополняем мантиссу нулями до размера в 30 значащих чисел (может повлиять на точность деления)
		data->mant[j++] = '0';
	data->mant[j] = '\0';
	// работа с "Е"
	int sign = 1; // переменная, отвечающая за знак степени десятки (знак значения "Е")
	if (str[i] == 'E') // переходим к считыванию значения степени десятки
		i++;
	if (str[i] == '-') // проверяем знак "Е"
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+') // пропускаем, если "+"
		i++;
	while (str[i]) // пока считываются цифры, собираем значение "Е"
	{
		data->exp_num = data->exp_num * 10 + (int)(str[i++] - '0');
	}
	data->exp_num += inc;
	data->exp_num *= sign; // не забываем про знак
	// преобразование к виду "(.)0xxxxxxxxxxxxx"
	int count = 0;
	while (data->mant[count] != '.')
		count++; // ищем позицию точки
	data->exp_num += count; // увеличиваем значение порядка
	while (count > 0)
	{
		data->mant[count] = data->mant[count - 1]; // смещаем точку и получаем из "ххх.ххххх" ---> "0ххххххххххх"
		count--;
	}
	// p.s.: точку в мантиссе я решил заменить на доп разряд, чтобы следить за переполнением
	// таким образом имеем "0*" вместо ".*"
	data->mant[count] = '0';
	return OK;
}

int mant_more_eq(const char *const mant1, const char *const mant2)
{
	int i, diff;
	for (i = 0; mant1[i]; i++) // проходим по мантиссам
	{
		if ((diff = (int)(mant1[i] - mant2[i])) < 0) // если цифра во второй мантиссе больше, значит второе число больше
			return NO;
		else if (diff > 0) // в таком случае больше первое число
			return YES;
	}
	// числа равны
	return YES;
}


int mant_diff(char *const mant1, const char *const mant2)
{
	if (!mant_more_eq(mant1, mant2)) // невозможно сделать вычитание, так как второе число больше
		return NO;
	int i = strlen(mant2) - 1;
	for (; i >= 0; i--) // классическая процедура вычитания в столбик
	{
		if (mant1[i] - mant2[i] >= 0) // если меньшему разряду не нужно занимать из более высокого разряда десятку
			mant1[i] -= mant2[i] - '0';
		else
		{
			int j = i - 1; // ищем, у кого занять
			while (mant1[j] - '0' == 0) 
				j--;
			mant1[j]--;
			for (j += 1; j > i; j++) //обновляем разряды
				mant1[j] += 9;
			mant1[i] += 10 - (mant2[i] - '0'); // делаем вычитание в разряде-заемщике
		}
	}
	return YES;
} 


void move_mant(big_num *const var, const int k)
{
	for (int i = 0; i < MANT_SIZE - 1 - k; i++)
		var->mant[i] = var->mant[i+k];
	for (int j = MANT_SIZE - 1 - k; j < MANT_SIZE - 1; j++)
		var->mant[j] = '0';
}


int div_iter(big_num *const a, big_num b)
{
	int iter = 0;
	while (mant_diff(a->mant, b.mant)) // считаем, сколько раз можно вычесть второе число из первого (делим нацело первое число на второе)
		iter++;
	move_mant(a, 1); // сдвигаем мантиссу (аналогично тому, как происходит деление в столбик)
	return iter;
}

int is_zero(const char *const var)
{
	int i = 0;
	for (i = 0; var[i] == '0'; i++);
	return (!var[i]) ? YES : NO;
}


big_num big_division(big_num a, big_num b)
{
	a.mant[0] = '0';
	b.mant[0] = '0';
	big_num res;
	res.is_bad = 0;
	// проверяем знаменатель на равенство нулю
	if (is_zero(b.mant))
	{
		res.is_bad = 1;
		return res;
	}
	// проверяем, является ли делимое нулем
	if (is_zero(a.mant))
	{
		strcpy(res.mant, "00\0");
		res.exp_num = 0;
		return res;
	}
	if (abs(a.exp_num - b.exp_num) > 99999) // проверка выхода степени числа за пределы области определения
	{
		res.is_bad = 1;
		return res;
	}
	// начинаем деление в столбик (вынося первую операцию за скобку, чтобы не было 0 после точки)
	int temp = 0; // переменная для хранения результата деления текущей стадии (очередная цифра результата)
	if (!mant_more_eq(a.mant, b.mant)) // если делимое меньше, чем делитель, то берем из степени единицу и домножаем мантиссу на 10
	{
		a.exp_num -= 1;
		move_mant(&a, 1);
	}
	temp = div_iter(&a, b); // находим первую цифру результата (если мантисса делимого равна нулю, то деление прекращается)
	res.mant[0] = temp + '0';
	for (int i = 1; i < MANT_LEN; i++) // находим все остальные цифры путем деления в столбик (деление так же прекращается, если мантисса делимого = 0)
	{
		if (is_zero(a.mant)) // если остаток после последнего деления = 0, то числа поделились нацело: конец
		{
			res.mant[i] = '\0';
			temp = 0;
			break;
		}
		if (!mant_more_eq(a.mant, b.mant)) // если не вычитается, то берем больше, а в результат идет 0
		{
			move_mant(&a, 1);
			res.mant[i] = '0';
		}
		else // если возможно - делим (вычитаем максимально возможное количество раз)
		{
			temp = div_iter(&a, b);
			res.mant[i] = '0' + temp;
		}
	}
	if ((temp = div_iter(&a, b)) > 5)
		res.mant[MANT_LEN - 1] += 1;
	for (int i = MANT_LEN; i > 0; i--)
		res.mant[i] = res.mant[i - 1];
	res.mant[0] = '0';
	res.mant[MANT_SIZE - 1] = '\0';
	res.sign_m = (a.sign_m * b.sign_m) % 2;
	res.exp_num = a.exp_num - b.exp_num + 1; // прибавка единицы связана с тем, 
	// что мы уменьшаем порядок во время сдвига результата вправо
	return res;
}


void remove_zeros(char *const str) 
{
	int i, len = strlen(str); 
	for (i = len - 1; str[i] == '0' && i > 1; i--);
	str[i + 1] = '\0';
}


void big_print(big_num var)
{
	if (var.sign_m == MINUS)
		printf("-");
	remove_zeros(var.mant);
	printf("0.%s", &var.mant[1]);
	if (var.exp_num != 0)
		printf("E%d", var.exp_num);
}


void pretty_print(const big_num *const data)
{
	printf("Result: ");
	big_print(data[0]);
	printf(" / ");
	big_print(data[1]);
	printf(" = ");
	big_print(data[2]);
	printf(";\n");
}
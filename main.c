#include <stdio.h>
#include <string.h>

#define MAXLEN 40
#define OK 0
#define DIV_ERROR 1
#define YES 1
#define NO 0
#define MINUS 1
#define PLUS 0

typedef char raw_data_arr[2][MAXLEN + 1]; // массив для сырых данных, то есть для введенных пользователем строк
typedef char num30[32]; // Тип "строка" для мантиссы
typedef struct // структура для хранения больших чисел
{
	short int sign_m : 1; // знак мантиссы
	num30 mant; // значение мантиссы
	int exp_num : 18; // степень десятки (значение после Е)
	unsigned int is_bad : 1; // бит ложен, если число испортилось в следствие деления (не лежит в заданном диапазоне)
} big_num;
typedef big_num big_nums_arr[3]; 


int my_gets(char *const str; const int max_len); // Функция ввода данных
int is_digit(const char c); // Функция проверки, является ли символ числом
int check_data(char *const str); // Функция проверки корректности ввода
int parse_raw_data(char *str, big_num *data); // Функция приведения строки к виду структуры хранения больших чисел
big_num big_division(big_num a; big_num b); // Функция деления больших чисел
void pretty_print(big_num *data); // Функция печати данных в нужном виде


int main(void)
{
	setbuf(stdout, NULL); // отключение буферизации

	raw_data_arr str_nums; 
	big_nums_arr data;

	// Получаем данные от пользователя и сразу проверяем корректность
	printf("Введите число (делимое) в формате с точкой (.00025, +123001., –123.456) или \
		экспоненциальном формате (1234567Е–20, 1234567Е20 или -123.4567Е23) и нажмите Enter:\n");
	my_gets(str_nums[0], MAXLEN);
	while (check_data(str_nums[0]))
	{
		printf("bad format. try again:\n");
		my_gets(str_nums[0], MAXLEN);
	}

	printf("Введите второе число (делитель) в том же формате и нажмите Enter:\n");
	my_gets(str_nums[1], MAXLEN);
	while (check_data(str_nums[1]))
	{
		printf("bad format. try again:\n");
		my_gets(str_nums[1], MAXLEN);
	}

	// Приводим данные к нужной структуре
	for (int i = 0; i < 2; i++)
		parse_raw_data(str_nums[i], &data[i]);

	// Выполняем деление и выводим результат
	data[2] = big_division(data[1], data[0]);
	if (data[2].is_correct)
	{
		prepare_data(data);
		printf("Результат: %se%d / %se%d = %se%d;", data[0].mant);
	}
	else
	{
		printf("В результате деления получено некорректное число!");
		return DIV_ERROR;
	}
	return OK;
}


int my_gets(char *const str; const int max_len) // 2ой параметр - ограничение на длину строки
{
	int k = 0;
	char c;
	while ((c = getchar()) == ' '); // пропускаем пробелы при вводе пользователя (игнорируем)
	while (k < max_len && (c = getchar()) != '\n' && c != EOF && c != '\0') // пока не конец, не Enter и не достигнуто ограничение
		str[k++] = c;
	str[k] = '\0';
	if (k == max_len) // если достигнут лимит длины строки, то до конца строки (файла) пропускаем остальные символы
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
	{
		switch (stage) 
		{
			case 1:
				if (str[i] == '+' || str[i] == '-' || is_digit(str[i])) // прочитан знак, следующая цифра (или точка..)
					stage = 2;
				else if (str[i] == '.') // прочитана точка, пропускаем первую стадию с цифрой
					stage = 3;
				else if (str[i] == 'E') // мантиссы не ожидается, переход к стадии ввода знака и значения степени десятки
					stage = 5;
				else
					return CHECK_ERROR; // *не один сценарий не случился*
				break;

			case 2:
				if (str[i] == 'E')
					stage = 5;
				else if (str[i] == '.')
					stage = 3;
				else if (!is_digit(str[i]))
					return CHECK_ERROR;
				break;

			case 3: // стадия "после точки". ожидается число. Если число получено, то переход к стадии ожидания "Е"
				if (!is_digit(str[i]))
					return CHECK_ERROR;
				stage = 4;
				break;

			case 4: // стадия ожидания "Е"
				if (str[i] == 'E') 
					stage = 5;
				else if (!is_digit(str[i]))
					return CHECK_ERROR;
				break;

			case 5: // стадия ожидания знака или перехода к проверки значения "Е"
				if (str[i] == '+' || str[i] == '-' || is_digit(str[i]))
					stage = 6;
				else
					return CHECK_ERROR;
				break;

			case 6: // стадия окончательной проверки "Е"
				if (!is_digit(str[i]))
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
	data->sign_m = (str[i] == '-') ? MINUS : PLUS; // знак мантиссы (если есть). Записываем в отведенное поле структуры
	if (str[i] == '+' || str[i] == '-') // знак учтен, пропускаем, переходя к значению
		i++;
	else if (str[i] == 'E') // если знака и значения не было, а была "Е", то наше значение - 1
		strcpy(data->mant, "1.\0");
	else
		data->mant[j++] = str[i++]; // иначе - записываем значения
	while (is_digit(str[i]) || str[i] == '.') // все еще записываем
		data->mant[j++] = str[i++];
	data->mant[j] = '\0';

	// работа с "Е"
	int sign = 1; // переменная, отвечающая за знак степени десятки (знак значения "Е")
	if (str[i] == 'E') // переходим к считыванию значения
		i++;
	if (str[i] == '-') // проверяем знак "Е"
		sign = -1;
	else if (str[i] == '+') // пропускаем, если "+"
		i++;
	while (str[i]) // пока считываются цифры, собираем значение "Е"
		data->exp_num = data->exp_num * 10 + (int)(str[i] - '0')
	data->exp_num *= sign; // не забываем про знак

	// преобразование к виду ".xxxxxxxxxxxxx"
	int count = 0;
	while (data->mant[count++] != '.'); // ищем позицию точки
	data->exp_num += count; // увеличиваем значение степени десятки
	while (count-- > 0)
		data->mant[count + 1] = data->mant[count]; // смещаем точку и получаем из "ххх.ххххх" ---> ".ххххххххххх"
	data.mant[count] = '.';

	return OK;
}


big_num big_division(big_num a; big_num b);


void pretty_print(big_num *data)
{
	printf("Result: ");
}
/*
 * Float_transform.c
 *
 * Простенькая библиотека для разбития float на две части: целую и дробную.
 * Это нужно, чтоб не использовать -u_printf_float в линкере, дабы сэкономить память МК.
 * В библиотеке используется округление.
 * Пример Val = 25.436f. Мы хотим вывести число с 2 знаками после запятой. Получим 25.44
 *
 *  Created on: Aug 18, 2021
 *      Author: Oleg Volkov
 */

#include "Float_transform.h"

/*--------------------------------Переменные для работы-------------------------------------*/
int integer_number = 0;
uint32_t fractional_number = 0;
/*--------------------------------Переменные для работы-------------------------------------*/

/*--------------------------------Скопировать в main.c--------------------------------------*/
/*extern int integer_number;
 extern uint32_t fractional_number;*/
/*--------------------------------Скопировать в main.c--------------------------------------*/

/*============================Пример работы с библиотекой===================================*/
//Файл main.c
/*
 #include "Float_transform.h"
 extern int integer_number;
 extern uint32_t fractional_number;

 float Val = -3.14159265359;

 Float_transform(Val, 4, &integer_number, &fractional_number);
 printf("float = %d.%.04d\r\n", integer_number, fractional_number);*/
//Получим вывод: float = -3.1416
/*
 * Обратите внимание на вывод чисел. Сколько знаков после запятой, столько и отступ дробного числа при печати.
 * Т.к. если Val = 25.0126;
 * и мы сделаем
 * Float_transform(Val, 3, &integer_number, &fractional_number);
 * то
 * integer_number = 25;
 * fractional_number = 13;
 * а вывод должен быть 25.013, поэтому:
 * printf("float = %d.%.03ld\r\n", integer_number, fractional_number);
 *
 * Получим вывод: float = 25.013
 *
 */
/*============================Пример работы с библиотекой===================================*/

/*--------------------------------Разбитие float с округлением------------------------------*/
void Float_transform(float value, uint8_t width, int *integer_number, uint32_t *fractional_number) {
/// Функция разбирия числа float на две части: целую и дробную.
	/// \param float value - число float, которое хотим разбить
	/// \param uint8_t width - сколько знаков после запятой хотим выводить
	/// \param int *integer_number - глобальная переменная для вещественной части
	/// \param uint32_t *fractional_number - глобальная переменная для дробной части

	float rounding = 0.05f;
	uint32_t rounding_2 = 10;

	if (value >= 0) {
		if (width == 0) {
			*integer_number = (int) value;
			*fractional_number = 0;
		} else if (width >= 1) {
			for (uint8_t i = 1; i < width; i++) {
				rounding = rounding * 0.1f;
				rounding_2 = rounding_2 * 10;
			}
			*integer_number = (int) ((float) value + rounding);

			*fractional_number = (((float) value + rounding) * rounding_2) - (*integer_number * rounding_2);
		}
	} else {
		if (width == 0) {
			*integer_number = (int) value;
			*fractional_number = 0;
		} else if (width >= 1) {
			for (uint8_t i = 1; i < width; i++) {
				rounding = rounding * 0.1f;
				rounding_2 = rounding_2 * 10;
			}
			*integer_number = (int) ((((float) value * (-1)) + rounding) * (-1));
			*fractional_number = ((((float) value * (-1)) + rounding) * rounding_2) - (*integer_number * (-1) * rounding_2);
		}
	}
}
/*--------------------------------Разбитие float с округлением------------------------------*/

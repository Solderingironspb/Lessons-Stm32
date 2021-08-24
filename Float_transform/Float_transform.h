/*
 * Float_transform.h
 *
 * v1.1(Исправление ошибки, когда -1.00f < float value < 0.00f)
 *
 * Простенькая библиотека для разбития float на две части: целую и дробную.
 * Это нужно, чтоб не использовать -u_printf_float в линкере, дабы сэкономить память МК.
 * В библиотеке используется округление.
 * Пример Val = 25.436f. Мы хотим вывести число с 2 знаками после запятой. Получим 25.44
 *
 *  Created on: Aug 24, 2021
 *      Author: Oleg Volkov
 *
 *      P.S Хочу сказать огромное спасибо Дмитрию Мачневу за тестирование и подсказки по оптимизации!
 *      В будущем может еще чего переделаем.
 */

#ifndef INC_FLOAT_TRANSFORM_H_
#define INC_FLOAT_TRANSFORM_H_

#include <main.h>

void Float_transform(float value, uint8_t width, uint8_t *sign_number, int *integer_number, uint32_t *fractional_number);

#endif /* INC_FLOAT_TRANSFORM_H_ */

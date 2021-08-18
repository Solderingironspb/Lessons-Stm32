/*
 * Float_transform.h
 *
 * Простенькая библиотека для разбития float на две части: целую и дробную.
 * Это нужно, чтоб не использовать -u_printf_float в линкере, дабы сэкономить память МК.
 * В библиотеке используется округление.
 * Пример Val = 25.436f. Мы хотим вывести число с 2 знаками после запятой. Получим 25.44
 *
 *  Created on: Aug 18, 2021
 *      Author: Oleg Volkov
 */

#ifndef INC_FLOAT_TRANSFORM_H_
#define INC_FLOAT_TRANSFORM_H_

#include <main.h>

void Float_transform(float value, uint8_t width, int *integer_number, uint16_t *fractional_number);

#endif /* INC_FLOAT_TRANSFORM_H_ */

/*
 * max7219_lib.c
 *
 *  Created on: Sep 27, 2020
 *      Author: Oleg Volkov
 */
#include "max7219_lib.h"
extern SPI_HandleTypeDef hspi1;

#define cs_set() GPIOA->BRR = CS_Pin; //опускаем ногу CS
#define cs_reset() GPIOA->BSRR = CS_Pin; //поднимаем ногу CS
#define _0 0x7E; //0
#define _1 0x30; //1
#define _2 0x6D; //2
#define _3 0x79; //3
#define _4 0x33; //4
#define _5 0x5B; //5
#define _6 0x5F; //6
#define _7 0x70; //7
#define _8 0x7F; //8
#define _9 0x7B; //9
#define _254 0x01; //-
#define _255 0x00; //пустота
#define _0_point 0xFE; //0.
#define _1_point 0xB0; //1.
#define _2_point 0xED; //2.
#define _3_point 0xF9; //3.
#define _4_point 0xB3; //4.
#define _5_point 0xDB; //5.
#define _6_point 0xDF; //6.
#define _7_point 0xF0; //7.
#define _8_point 0xFF; //8.
#define _9_point 0xFB; //9.

uint8_t symbol[8] = { 0, };

/*----------Общение с max7219 по spi-----------------*/
///Общение с max7219 по spi(выбор регистра и отправка данных).
///(См. Datasheet MAX7219/MAX7221. Cтр.7, табл. 2).
/// \param adress - адрес регистра, в который производится отправка данных;
/// \param data - данные.
void max7219_Transmit(uint8_t adress, uint8_t data) {
	uint8_t tx_buffer[1] = { 0 };
	cs_set();
	tx_buffer[0] = adress;
	HAL_SPI_Transmit(&hspi1, tx_buffer, 1, 100);
	tx_buffer[0] = data;
	HAL_SPI_Transmit(&hspi1, tx_buffer, 1, 100);
	cs_reset();
}
/*----------Общение с max7219 по spi-----------------*/

/*--------------------Режим сна----------------------*/
///Режим сна.(См. Datasheet MAX7219/MAX7221. Стр.7, табл. 3).
/// \param data - данные. 0 - Режим сна. 1 - Нормальный режим.
void max7219_Shutdown(bool data){
	max7219_Transmit(0x0C, data);
}
/*--------------------Режим сна----------------------*/

/*-------------------Тест дисплея--------------------*/
///Тест дисплея.(См. Datasheet MAX7219/MAX7221. Стр.10, табл. 10).
/// \param data - данные. 0 - Нормальный режим. 1 - Тест дисплея.
void max7219_Display_Test(bool data){
	max7219_Transmit(0x0F, data);
}
/*-------------------Тест дисплея--------------------*/

/*-----------------Очистка дисплея-------------------*/
///Очистка дисплея. (См. мой урок https://youtu.be/BonEEaQX8vg).
void max7219_Display_Clean(void){
	max7219_Transmit(0x01, 0x00);
	max7219_Transmit(0x02, 0x00);
	max7219_Transmit(0x03, 0x00);
	max7219_Transmit(0x04, 0x00);
	max7219_Transmit(0x05, 0x00);
	max7219_Transmit(0x06, 0x00);
	max7219_Transmit(0x07, 0x00);
	max7219_Transmit(0x08, 0x00);
}
/*-----------------Очистка дисплея-------------------*/

/*-----------------Яркость дисплея-------------------*/
///Яркость дисплея.(См. Datasheet MAX7219/MAX7221. Стр.9, табл. 7).
/// \param data - данные. Значения от 0x00 до 0x0F.
void max7219_Intensity(uint8_t data){
max7219_Transmit(0x0A, data);
}
/*-----------------Яркость дисплея-------------------*/

/*--------------Инициализация дисплея----------------*/
///Инициализация дисплея.
void max7219_Init(void) {
	max7219_Transmit(0x09, 0x00); //Работа без режима декодирование
	max7219_Transmit(0x0B, 0x07); //Количество используемых разрядов
	max7219_Intensity(0x0F); //Интенсивность свечения на максимум
	max7219_Shutdown(1); //Вывести дисплей из сна в нормальный режим работы
	max7219_Display_Test(1); //Проверка дисплея
	HAL_Delay(2000); //Время на визуальный осмотр
	max7219_Display_Test(0); //Перевод дисплея в нормальный режим работы
	max7219_Display_Clean(); //Очистка дисплея
}
/*--------------Инициализация дисплея----------------*/

/*-----------Отправка символов на дисплей------------*/
///Отправка символов на дисплей. Функция вспомогательная.
/// \param *tx_buffer - Массив с данными, которые будут выводиться на дисплей.
void max7219_send_symbol(uint8_t *tx_buffer) {
	max7219_Transmit(8, tx_buffer[0]);
	max7219_Transmit(7, tx_buffer[1]);
	max7219_Transmit(6, tx_buffer[2]);
	max7219_Transmit(5, tx_buffer[3]);
	max7219_Transmit(4, tx_buffer[4]);
	max7219_Transmit(3, tx_buffer[5]);
	max7219_Transmit(2, tx_buffer[6]);
	max7219_Transmit(1, tx_buffer[7]);
}
/*-----------Отправка символов на дисплей------------*/

/*---Подготовка данных к выводу на дисплей. Цифры, минус, пробел---*/
///Подготовка данных к выводу на дисплей. Цифры, минус, пробел. Функция вспомогательная.
void max7219_num(uint8_t rg, uint32_t value) {
	switch (value) {
	case 0:
		symbol[rg] = _0;
		break;
	case 1:
		symbol[rg] = _1;
		break;
	case 2:
		symbol[rg] = _2;
		break;
	case 3:
		symbol[rg] = _3;
		break;
	case 4:
		symbol[rg] = _4;
		break;
	case 5:
		symbol[rg] = _5;
		break;
	case 6:
		symbol[rg] = _6;
		break;
	case 7:
		symbol[rg] = _7;
		break;
	case 8:
		symbol[rg] = _8;
		break;
	case 9:
		symbol[rg] = _9;
		break;
	case 254:
		symbol[rg] = _254;
		break;
	case 255:
		symbol[rg] = _255;
		break;
	}
}
/*---Подготовка данных к выводу на дисплей. Цифры, минус, пробел---*/

/*---Подготовка данных к выводу на дисплей. Цифры с точкой---*/
///Подготовка данных к выводу на дисплей. Цифры с точкой. Функция вспомогательная.
void max7219_num_point(uint8_t rg, uint32_t value) {
	switch (value) {
	case 0:
		symbol[rg] = _0_point;
		break;
	case 1:
		symbol[rg] = _1_point;
		break;
	case 2:
		symbol[rg] = _2_point;
		break;
	case 3:
		symbol[rg] = _3_point;
		break;
	case 4:
		symbol[rg] = _4_point;
		break;
	case 5:
		symbol[rg] = _5_point;
		break;
	case 6:
		symbol[rg] = _6_point;
		break;
	case 7:
		symbol[rg] = _7_point;
		break;
	case 8:
		symbol[rg] = _8_point;
		break;
	case 9:
		symbol[rg] = _9_point;
		break;
	}
}
/*---Подготовка данных к выводу на дисплей. Цифры с точкой---*/

/*-------Отправка чисел без плавающей запятой на дисплей-------*/
///Отправка чисел без плавающей запятой на дисплей. Хоть функция и содержит в названии int32_t - это не значит,
///что отправляемые значения должны быть именно int32_t.
///Поддерживаются: uint8_t/int8_t
///                uint16_t/int16_t
///                uint32_t/int32_t
///Единственное ограничение - число должно быть в рамках от -9999999 до 99999999.
///В противном случае, на дисплее будет отображено "--------".
///Функция проверяет отправленное число на выход за рамки вывода дисплея, положительное оно или отрицательное,
///далее преобразует отправленное число в BCD код(двоично-десятичный код.) и уже после этого отправляет данные на дисплей.
void max7219_Send_int32_t(int32_t value) {
	if (value > 99999999 || value < -9999999) {
		max7219_num(0, 254);
		max7219_num(1, 254);
		max7219_num(2, 254);
		max7219_num(3, 254);
		max7219_num(4, 254);
		max7219_num(5, 254);
		max7219_num(6, 254);
		max7219_num(7, 254);
		max7219_send_symbol(symbol);
	} else {
		bool positive_value;
		if (value < 0) {
			positive_value = false;
			value = value * (-1);
		} else {
			positive_value = true;
		}
		uint8_t BCD_Arr[8] = { 0, };
		uint32_t lenght;
		lenght = value;
		while (value >= 10000000) {
			value -= 10000000;
			BCD_Arr[7]++;
		}

		while (value >= 1000000) {
			value -= 1000000;
			BCD_Arr[6]++;
		}
		while (value >= 100000) {
			value -= 100000;
			BCD_Arr[5]++;
		}
		while (value >= 10000) {
			value -= 10000;
			BCD_Arr[4]++;

		}
		while (value >= 1000) {
			value -= 1000;
			BCD_Arr[3]++;

		}
		while (value >= 100) {
			value -= 100;
			BCD_Arr[2]++;

		}
		while (value >= 10) {
			value -= 10;
			BCD_Arr[1]++;

		}
		BCD_Arr[0] = (uint8_t) (value);

		if (lenght < 10) {
			max7219_num(0, 255);
			max7219_num(1, 255);
			max7219_num(2, 255);
			max7219_num(3, 255);
			max7219_num(4, 255);
			max7219_num(5, 255);
			if (positive_value == true) {
				max7219_num(6, 255);
			} else {
				max7219_num(6, 254);
			}
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 100) {
			max7219_num(0, 255);
			max7219_num(1, 255);
			max7219_num(2, 255);
			max7219_num(3, 255);
			max7219_num(4, 255);
			if (positive_value == true) {
				max7219_num(5, 255);
			} else {
				max7219_num(5, 254);
			}
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);

		} else if (lenght < 1000) {
			max7219_num(0, 255);
			max7219_num(1, 255);
			max7219_num(2, 255);
			max7219_num(3, 255);
			if (positive_value == true) {
				max7219_num(4, 255);
			} else {
				max7219_num(4, 254);
			}
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 10000) {
			max7219_num(0, 255);
			max7219_num(1, 255);
			max7219_num(2, 255);
			if (positive_value == true) {
				max7219_num(3, 255);
			} else {
				max7219_num(3, 254);
			}
			max7219_num(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 100000) {
			max7219_num(0, 255);
			max7219_num(1, 255);
			if (positive_value == true) {
				max7219_num(2, 255);
			} else {
				max7219_num(2, 254);
			}
			max7219_num(3, BCD_Arr[4]);
			max7219_num(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 1000000) {
			max7219_num(0, 255);
			if (positive_value == true) {
				max7219_num(1, 255);
			} else {
				max7219_num(1, 254);
			}
			max7219_num(2, BCD_Arr[5]);
			max7219_num(3, BCD_Arr[4]);
			max7219_num(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);

		} else if (lenght < 10000000) {
			if (positive_value == true) {
				max7219_num(0, 255);
			} else {
				max7219_num(0, 254);
			}
			max7219_num(1, BCD_Arr[6]);
			max7219_num(2, BCD_Arr[5]);
			max7219_num(3, BCD_Arr[4]);
			max7219_num(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 100000000) {
			max7219_num(0, BCD_Arr[7]);
			max7219_num(1, BCD_Arr[6]);
			max7219_num(2, BCD_Arr[5]);
			max7219_num(3, BCD_Arr[4]);
			max7219_num(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		}
	}
}
/*-------Отправка чисел без плавающей запятой на дисплей-------*/

/*--------Отправка чисел с плавающей запятой на дисплей--------*/
///Отправка чисел с плавающей запятой на дисплей.
///Число должно быть в рамках от -9999.999 до 99999.999.
///В противном случае, на дисплее будет отображено "--------".
///Функция проверяет отправленное число на выход за рамки вывода дисплея, положительное оно или отрицательное,
///далее преобразует отправленное число в BCD код(двоично-десятичный код.) и уже после этого отправляет данные на дисплей.
void max7219_Send_float(float value) {
	if (value > 99999.999f || value < -9999.999f) {
		max7219_num(0, 254);
		max7219_num(1, 254);
		max7219_num(2, 254);
		max7219_num(3, 254);
		max7219_num(4, 254);
		max7219_num(5, 254);
		max7219_num(6, 254);
		max7219_num(7, 254);
		max7219_send_symbol(symbol);
	} else {
		int32_t value_conv = value * 1000;
		value = value * 1000;
		bool positive_value;
		if (value_conv < 0) {
			positive_value = false;
			value_conv = value_conv * (-1);
		} else {
			positive_value = true;
		}
		uint8_t BCD_Arr[8] = { 0, };
		int32_t lenght;
		lenght = value_conv;
		while (value_conv >= 10000000) {
			value_conv -= 10000000;
			BCD_Arr[7]++;
		}

		while (value_conv >= 1000000) {
			value_conv -= 1000000;
			BCD_Arr[6]++;
		}
		while (value_conv >= 100000) {
			value_conv -= 100000;
			BCD_Arr[5]++;
		}
		while (value_conv >= 10000) {
			value_conv -= 10000;
			BCD_Arr[4]++;

		}
		while (value_conv >= 1000) {
			value_conv -= 1000;
			BCD_Arr[3]++;

		}
		while (value_conv >= 100) {
			value_conv -= 100;
			BCD_Arr[2]++;

		}
		while (value_conv >= 10) {
			value_conv -= 10;
			BCD_Arr[1]++;

		}
		BCD_Arr[0] = (uint8_t) (value_conv);

		if (lenght < 10) {
			max7219_num(0, 255);
			max7219_num(1, 255);
			max7219_num(2, 255);
			if (positive_value == true) {
				max7219_num(3, 255);
			} else {
				max7219_num(3, 254);
			}
			max7219_num_point(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 100) {
			max7219_num(0, 255);
			max7219_num(1, 255);
			max7219_num(2, 255);
			if (positive_value == true) {
				max7219_num(3, 255);
			} else {
				max7219_num(3, 254);
			}
			max7219_num_point(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);

		} else if (lenght < 1000) {

			max7219_num(0, 255);
			max7219_num(1, 255);
			max7219_num(2, 255);
			if (positive_value == true) {
				max7219_num(3, 255);
			} else {
				max7219_num(3, 254);
			}
			max7219_num_point(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 10000) {
			max7219_num(0, 255);
			max7219_num(1, 255);
			max7219_num(2, 255);
			if (positive_value == true) {
				max7219_num(3, 255);
			} else {
				max7219_num(3, 254);
			}
			max7219_num_point(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 100000) {
			max7219_num(0, 255);
			max7219_num(1, 255);
			if (positive_value == true) {
				max7219_num(2, 255);
			} else {
				max7219_num(2, 254);
			}
			max7219_num(3, BCD_Arr[4]);
			max7219_num_point(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 1000000) {
			max7219_num(0, 255);
			if (positive_value == true) {
				max7219_num(1, 255);
			} else {
				max7219_num(1, 254);
			}
			max7219_num(2, BCD_Arr[5]);
			max7219_num(3, BCD_Arr[4]);
			max7219_num_point(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);

		} else if (lenght < 10000000) {
			if (positive_value == true) {
				max7219_num(0, 255);
			} else {
				max7219_num(0, 254);
			}
			max7219_num(1, BCD_Arr[6]);
			max7219_num(2, BCD_Arr[5]);
			max7219_num(3, BCD_Arr[4]);
			max7219_num_point(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		} else if (lenght < 100000000) {
			max7219_num(0, BCD_Arr[7]);
			max7219_num(1, BCD_Arr[6]);
			max7219_num(2, BCD_Arr[5]);
			max7219_num(3, BCD_Arr[4]);
			max7219_num_point(4, BCD_Arr[3]);
			max7219_num(5, BCD_Arr[2]);
			max7219_num(6, BCD_Arr[1]);
			max7219_num(7, BCD_Arr[0]);
			max7219_send_symbol(symbol);
		}
	}

}
/*--------Отправка чисел с плавающей запятой на дисплей--------*/

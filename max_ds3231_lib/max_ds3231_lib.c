/*
 * max_ds3231_lib.c
 *
 *  Created on: 14 нояб. 2020 г.
 *      Author: Oleg Volkov
 */

#include "max_ds3231_lib.h"

#define Adress 0xD0             //Адрес устройства

extern I2C_HandleTypeDef hi2c1; //Шина i2c. В данном примере используется шина i2c1.

/*-------------------------------------Глобальные переменные----------------------------------*/

uint8_t Seconds = 0;                   //Секунды
uint8_t Minutes = 0;                   //Минуты
uint8_t Hours = 0;                     //Часы
uint8_t Day = 0;                       //День недели
uint8_t Date = 0;                      //Дата
uint8_t Month = 0;                     //Месяц
uint8_t Cuntury = 0;                   //Век
uint16_t Year = 0;                     //Год
float max_ds3231_temp = 0.0f;          //Температура внутреннего датчика

/*-------------------------------------Глобальные переменные----------------------------------*/

/*--------------------------------Функция запроса данных о времени----------------------------*/

void max_ds3231_get_time(void) {
	///Функция запроса данных о времени/
	///(См. Datasheet DS3231. Cтр.11, табл. 1).
	///Считываем регистры с 0x00 по 0x06 включительно.
	
	uint8_t tx_buffer = 0x00;
        uint8_t rx_buffer[7] = { 0, };
	HAL_I2C_Master_Transmit(&hi2c1, Adress, &tx_buffer, 1, 10);
	HAL_I2C_Master_Receive(&hi2c1, Adress, rx_buffer, 7, 10);

	Seconds = (rx_buffer[0] >> 4) * 10 + (rx_buffer[0] & 0x0F);
	Minutes = (rx_buffer[1] >> 4) * 10 + (rx_buffer[1] & 0x0F);
	Hours = (rx_buffer[2] >> 4) * 10 + (rx_buffer[2] & 0x0F);
	Day = rx_buffer[3];
	Date = (rx_buffer[4] >> 4) * 10 + (rx_buffer[4] & 0x0F);
	Month = ((rx_buffer[5] >> 4) & 0x07) * 10 + (rx_buffer[5] & 0x0F);
	Cuntury = 20 + (rx_buffer[5] >> 7);
	Year = 2000 + (rx_buffer[6] >> 4) * 10 + (rx_buffer[6] & 0x0F);

	/*-----------------------Если включена отладка по SWO-------------------------------------*/
	/*printf("\r\n");
	 printf("Moscow time:\r\n");
	 printf("%02d:%02d:%02d\r\n", Hours, Minutes, Seconds);
	 printf("%02d:%02d:%d\r\n", Date, Month, Year);
	 printf("Cuntury = %d\r\n", Cuntury);*/
	/*-----------------------Если включена отладка по SWO-------------------------------------*/
}

/*--------------------------------Функция запроса данных о времени----------------------------*/

/*------------------------------Функция запроса данных о температуре--------------------------*/

void max_ds3231_get_temperature(void) {
	///Функция запроса данных о времени/
	///(См. Datasheet DS3231. Cтр.11, табл. 1).
	///Считываем регистры с 0x11 по 0x12 включительно.
	uint8_t rx_buffer[2] = { 0, };
	uint8_t tx_buffer = 0x11;
	float temp_fractional_part = 0.0f;
	HAL_I2C_Master_Transmit(&hi2c1, Adress, &tx_buffer, 1, 10);
	HAL_I2C_Master_Receive(&hi2c1, Adress, rx_buffer, 2, 10);

	max_ds3231_temp = 0.0f;
	temp_fractional_part = 0.0f;

	if (rx_buffer[1] == 0x00) {
		temp_fractional_part = 0.0f;
	} else if (rx_buffer[1] == 0x40) {
		temp_fractional_part = 0.25f;
	} else if (rx_buffer[1] == 0x80) {
		temp_fractional_part = 0.5f;
	} else if (rx_buffer[1] == 0xC0) {
		temp_fractional_part = 0.75f;
	}

	if (rx_buffer[0] < 0x80) {
		max_ds3231_temp = rx_buffer[0] + temp_fractional_part;
	} else {
		max_ds3231_temp = (rx_buffer[0] + temp_fractional_part) * (-1);
	}
	/*-----------------------Если включена отладка по SWO-------------------------------------*/
	//printf("Temperature: %.2f\r\n", max_ds3231_temp);
	/*-----------------------Если включена отладка по SWO-------------------------------------*/
}

/*------------------------------Функция запроса данных о температуре--------------------------*/

/*--------------------------------------Задать время(секунды)---------------------------------*/

void max_ds3231_set_seconds(uint8_t seconds) {
	///Функция: Задать время (секунды)
	///(См. Datasheet DS3231. Cтр.11, табл. 1).
	///Записываем данные в адрес 0x00.
	/// \param seconds - Секунды. Параметр от 0 до 59;
	uint8_t tx_buffer[2] = { 0, };
	tx_buffer[0] = 0x00;
	if (seconds < 10) {
		tx_buffer[1] = seconds % 10;
	} else if (seconds >= 10 && seconds <= 59) {
		tx_buffer[1] = (((seconds - (seconds % 10)) / 10) << 4) | (seconds % 10);
	} else if (seconds > 59) {
		tx_buffer[1] = 0;
	}

	HAL_I2C_Master_Transmit(&hi2c1, Adress, tx_buffer, 2, HAL_MAX_DELAY);
}

/*--------------------------------------Задать время(секунды)---------------------------------*/

/*--------------------------------------Задать время(минуты)---------------------------------*/

void max_ds3231_set_minutes(uint8_t minutes) {
	///Функция: Задать время (минуты)
	///(См. Datasheet DS3231. Cтр.11, табл. 1).
	///Записываем данные в адрес 0x01.
	/// \param minutes - Минуты. Параметр от 0 до 59;
	uint8_t tx_buffer[2] = { 0, };
	tx_buffer[0] = 0x01;
	if (minutes < 10) {
		tx_buffer[1] = minutes % 10;
	} else if (minutes >= 10 && minutes <= 59) {
		tx_buffer[1] = (((minutes - (minutes % 10)) / 10) << 4) | (minutes % 10);
	} else if (minutes > 59) {
		tx_buffer[1] = 0;
	}

	HAL_I2C_Master_Transmit(&hi2c1, Adress, tx_buffer, 2, HAL_MAX_DELAY);
}

/*--------------------------------------Задать время(минуты)---------------------------------*/

/*---------------------------------------Задать время(час)-----------------------------------*/

void max_ds3231_set_hours(uint8_t hours) {
	///Функция: Задать время (час)
	///(См. Datasheet DS3231. Cтр.11, табл. 1).
	///Записываем данные в адрес 0x02.
	/// \param hours - Часы. Параметр от 0 до 59;
	uint8_t tx_buffer[2] = { 0, };
	tx_buffer[0] = 0x02;
	if (hours < 10) {
		tx_buffer[1] = hours % 10;
	} else if (hours >= 10 && hours <= 23) {
		tx_buffer[1] = (((hours - (hours % 10)) / 10) << 4) | (hours % 10);
	} else if (hours > 23) {
		tx_buffer[1] = 0;
	}

	HAL_I2C_Master_Transmit(&hi2c1, Adress, tx_buffer, 2, HAL_MAX_DELAY);
}

/*---------------------------------------Задать время(час)-----------------------------------*/

/*----------------------------------Задать время(день недели)--------------------------------*/

void max_ds3231_set_day(uint8_t day) {
	///Функция: Задать время (день недели)
	///(См. Datasheet DS3231. Cтр.11, табл. 1).
	///Записываем данные в адрес 0x03.
	/// \param day - День недели. Пн = 1, Вт = 2 и т.д. Параметр от 1 до 7;
	uint8_t tx_buffer[2] = { 0, };
	tx_buffer[0] = 0x03;
	if (day > 0 && day <= 7) {
		tx_buffer[1] = day;
	} else {
		tx_buffer[1] = 1;
	}

	HAL_I2C_Master_Transmit(&hi2c1, Adress, tx_buffer, 2, HAL_MAX_DELAY);
}

/*----------------------------------Задать время(день недели)--------------------------------*/

/*--------------------------------------Задать время(дата)-----------------------------------*/

void max_ds3231_set_date(uint8_t date) {
	///Функция: Задать время (дата)
	///(См. Datasheet DS3231. Cтр.11, табл. 1).
	///Записываем данные в адрес 0x04.
	/// \param date - Дата. Параметр от 1 до 31;
	uint8_t tx_buffer[2] = { 0, };
	tx_buffer[0] = 0x04;
	if (date > 0 && date < 10) {
		tx_buffer[1] = date % 10;
	} else if (date >= 10 && date <= 31) {
		tx_buffer[1] = (((date - (date % 10)) / 10) << 4) | (date % 10);
	} else if (date == 0 || date > 31) {
		tx_buffer[1] = 1;
	}

	HAL_I2C_Master_Transmit(&hi2c1, Adress, tx_buffer, 2, HAL_MAX_DELAY);
}

/*--------------------------------------Задать время(дата)-----------------------------------*/

/*-----------------------------------Задать время (месяц, век)-------------------------------*/

void max_ds3231_set_month_cuntury(uint8_t month, uint8_t cuntury) {
	///Функция: Задать время (месяц, век)
	///(См. Datasheet DS3231. Cтр.11, табл. 1).
	///Записываем данные в адрес 0x05.
	/// \param mounth - Месяц. Параметр от 1 до 12;
	/// \param cuntury - Век. Параметр от 20 до 21;
	uint8_t tx_buffer[2] = { 0, };
	tx_buffer[0] = 0x05;
	if (month > 0 && month < 10) {
		tx_buffer[1] = month % 10;
	} else if (month >= 10 && month <= 12) {
		tx_buffer[1] = (((month - (month % 10)) / 10) << 4) | (month % 10);
	} else if (month == 0 || month > 12) {
		tx_buffer[1] = 1;
	}

	if (cuntury <= 20) {
		tx_buffer[1] = tx_buffer[1] & 0x7F;
	} else if (cuntury >= 21) {
		tx_buffer[1] = tx_buffer[1] | 0x80;
	}

	HAL_I2C_Master_Transmit(&hi2c1, Adress, tx_buffer, 2, HAL_MAX_DELAY);
}

/*-----------------------------------Задать время (месяц, век)-------------------------------*/

/*---------------------------------------Задать время (год)----------------------------------*/

void max_ds3231_set_year(uint16_t year) {
	///Функция: Задать время (год)
	///(См. Datasheet DS3231. Cтр.11, табл. 1).
	///Записываем данные в адрес 0x06.
	/// \param year - Год. Параметр от 1900 до 2099;
	uint8_t tx_buffer[2] = { 0, };
	tx_buffer[0] = 0x06;

	if (year >= 1900 && year <= 1999) {
		year = year - 1900;
	} else if (year >= 2000 && year <= 2099) {
		year = year - 2000;
	} else {
		year = 0;
	}

	if (year < 10) {
		tx_buffer[1] = year % 10;
	} else if (year >= 10 && year <= 99) {
		tx_buffer[1] = (((year - (year % 10)) / 10) << 4) | (year % 10);
	} else if (year > 99) {
		tx_buffer[1] = 0;
	}

	HAL_I2C_Master_Transmit(&hi2c1, Adress, tx_buffer, 2, HAL_MAX_DELAY);
}

/*---------------------------------------Задать время (год)----------------------------------*/

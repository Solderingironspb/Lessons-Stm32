/*
 * uart_mqtt_lib.c
 *
 *  Created on: 2 сент. 2020 г.
 *      Author: Oleg Volkov
 */

//****************************************************************************************//
//ВНИМАНИЕ! Для работы с флагом IDLE нужно сделать следующее:
//В файл stm32f1xx_hal_uart.c , расположенный в Drivers/STM32F1xx_HAL_Driver/Src
//конец цикла void HAL_UART_IRQHandler(UART_HandleTypeDef *huart). Это будет примерно 2161 строка:
//Добавить:
/* UART_IDLE_Callback —------------------------------------------*/
/*if (((isrflags & USART_SR_IDLE) != RESET) && ((cr1its & USART_CR1_IDLEIE) != RESET))
 {
 HAL_UART_IDLE_Callback(huart);
 return;
 }
 */

//P.S. Расскомменитировать код не забудь :)
//После каждой новой генерации в CubeIDE/CubeMX, данные строки будут пропадать. Поэтому имей это ввиду.
//Прототип функции HAL_UART_IDLE_Callback(huart) уже добавлен в uart_mqtt_lib.h данной библиотеки.
//****************************************************************************************//
/*----------------------Добавить в main.c------------------------*/
/*
 #include <stdio.h>
 #include <stdbool.h>
 #include "uart_mqtt_lib.h"
 #define esp_rx_buffer_size 256 //Размер входящего буфера. Сделан большим, т.к. ESP, при включении, отправляет сервисные команды, там много текста на другой частоте. Мы это отфильтруем.
 #define VARIABLE_ID esp_rx_buffer[1] //Номер переменной.
 extern bool MQTT_flag, WIFI_flag; //Флаги для отслеживания подключения к Wi-fi и MQTT серверу
 extern uint8_t esp_rx_buffer[esp_rx_buffer_size]; //Входящий буфер данных
 extern uint8_t esp_tx_buffer[9]; //Исходящий буфер данных
 extern uint16_t esp_rx_buffer_len; //Переменная для определения длины входящего сообщения
*/

/*======================Функция парсинга входящих данных с UART======================*/
//void UART_MQTT_Receive_Callback(void) {
	/*----------основные действия с входящими данными-----------*/
	//switch (VARIABLE_ID) {
	/*----------------здесь будет выборка по номеру переменной-----------------------*/

	//case (0xFF):
		//UART_MQTT_Check_net();
		//break;

		/*----------------здесь будет выборка по номеру переменной-----------------------*/
	//}
	/*----------основные действия с входящими данными-----------*/
//}
/*======================Функция парсинга входящих данных с UART======================*/



/*#############################################################################*/
/*------------------------IDLE_Callback---------------------------------------*/
/*void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart) {
	if (huart == &huart1) { //проверка, из нужного ли uart пришли данные
		UART_MQTT_IDLE_Callback_setting();
	}
}*/
/*------------------------IDLE_Callback---------------------------------------*/

/*------------------------RxCpltCallback---------------------------------------*/
/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart1) { //проверка, из нужного ли uart пришли данные
		UART_MQTT_RxCpltCallback_setting();
	}
}*/
/*------------------------RxCpltCallback---------------------------------------*/
/*#############################################################################*/



/*----------------------Добавить в main.c------------------------*/

#include "uart_mqtt_lib.h"
#define huart_used huart1 //указываем используемый UART для общения с ESP.
extern UART_HandleTypeDef huart1;
#define UART_MQTT_RESET_HIGH() GPIOA->BSRR = RESET_Pin; //Управление ножкой RESET
#define UART_MQTT_RESET_LOW() GPIOA->BSRR = (uint32_t) RESET_Pin << 16u; //Управление ножкой RESET
#define esp_rx_buffer_size 256 //Размер входящего буфера. Сделан большим, т.к. ESP, при включении, отправляет сервисные команды, там много текста на другой частоте. Мы это отфильтруем.

bool MQTT_flag, WIFI_flag; //Флаги для отслеживания подключения к Wi-fi и MQTT серверу
uint8_t esp_rx_buffer[esp_rx_buffer_size] = { 0, }; //Входящий буфер данных
uint8_t esp_tx_buffer[9] = { 0, }; //Исходящий буфер данных
uint16_t esp_rx_buffer_len; //Переменная для определения длины входящего сообщения


/*----------Инициализация UART_MQTT-----------------*/
/// Инициализация UART_MQTT протокола.
/// Основано на работе с UART и флагом IDLE.
/// (см. урок #9(UART, прием данных, не зная размера данных. UART_IT_IDLE. Работа с прерываниями.)
/// Ссылка на урок: https://youtu.be/2qkGNR6aKGY
void UART_MQTT_init(void) {
	MQTT_flag = 0;
	WIFI_flag = 0;
	__HAL_UART_ENABLE_IT(&huart_used, UART_IT_IDLE);
	HAL_UART_Receive_IT(&huart_used, esp_rx_buffer, esp_rx_buffer_size);
	UART_MQTT_RESET_HIGH();
	HAL_Delay(100);
	UART_MQTT_RESET_LOW();
}
/*----------Инициализация UART_MQTT-----------------*/

/*----------Проверка подключения ESP8266 к Wifi и MQTT брокеру-----------------*/
/// Проверка подключения ESP8266 к Wifi и MQTT брокеру
void UART_MQTT_Check_net(void) {
	if (UART_MQTT_Checksumm_validation(esp_rx_buffer)) {
		if ((esp_rx_buffer[0] == 0xFF) && (esp_rx_buffer[1] == 0xFF) && (esp_rx_buffer[2] == 0xFF) && (esp_rx_buffer[3] == 0xBB) && (esp_rx_buffer[4] == 0xBB) && (esp_rx_buffer[5] == 0xBB)
				&& (esp_rx_buffer[6] == 0xBB)) {
			WIFI_flag = 0;
			printf("Wi_fi != OK\r\n");
		} else if ((esp_rx_buffer[0] == 0xFF) && (esp_rx_buffer[1] == 0xFF) && (esp_rx_buffer[2] == 0xFF) && (esp_rx_buffer[3] == 0xAA) && (esp_rx_buffer[4] == 0xAA) && (esp_rx_buffer[5] == 0xAA)
				&& (esp_rx_buffer[6] == 0xAA)) {
			WIFI_flag = 1;
			printf("Wi_fi == OK\r\n");
		} else if ((esp_rx_buffer[0] == 0xFF) && (esp_rx_buffer[1] == 0xFF) && (esp_rx_buffer[2] == 0xFF) && (esp_rx_buffer[3] == 0xDD) && (esp_rx_buffer[4] == 0xDD) && (esp_rx_buffer[5] == 0xDD)
				&& (esp_rx_buffer[6] == 0xDD)) {
			MQTT_flag = 0;
			printf("MQTT != OK\r\n");
		} else if ((esp_rx_buffer[0] == 0xFF) && (esp_rx_buffer[1] == 0xFF) && (esp_rx_buffer[2] == 0xFF) && (esp_rx_buffer[3] == 0xCC) && (esp_rx_buffer[4] == 0xCC) && (esp_rx_buffer[5] == 0xCC)
				&& (esp_rx_buffer[6] == 0xCC)) {
			MQTT_flag = 1;
			printf("MQTT == OK\r\n");
		}
	}
}
/*----------Проверка подключения ESP8266 к Wifi и MQTT брокеру-----------------*/

/*---------Сервисная функция. HAL_UART_IDLE_Callback. Сработка флага IDLE---------*/
/// Сервисная функция. HAL_UART_IDLE_Callback. Сработка флага IDLE
/// UART_MQTT_Receive_Callback() - функция. Которую мы прописываем в main.c
/// В ней будем парсить входящие данные.
void UART_MQTT_IDLE_Callback_setting(void) {
	__HAL_UART_DISABLE_IT(&huart_used, UART_IT_IDLE);
	esp_rx_buffer_len = esp_rx_buffer_size - huart_used.RxXferCount;

	UART_MQTT_Receive_Callback();

	HAL_UART_AbortReceive_IT(&huart_used);
	__HAL_UART_CLEAR_IDLEFLAG(&huart_used);
	__HAL_UART_ENABLE_IT(&huart_used, UART_IT_IDLE);
	HAL_UART_Receive_IT(&huart_used, esp_rx_buffer, esp_rx_buffer_size);
}
/*---------Сервисная функция. HAL_UART_IDLE_Callback. Сработка флага IDLE---------*/

/*---------Сервисная функция. RxCpltCallback. Переполнение rx буфера---------*/
/// Сервисная функция. RxCpltCallback. Переполнение rx буфера
void UART_MQTT_RxCpltCallback_setting(void) {
	__HAL_UART_DISABLE_IT(&huart_used, UART_IT_IDLE);
	//printf("rx_buffer is overflow\n\r");
	HAL_UART_AbortReceive_IT(&huart_used);
	__HAL_UART_CLEAR_IDLEFLAG(&huart_used);
	__HAL_UART_ENABLE_IT(&huart_used, UART_IT_IDLE);
	HAL_UART_Receive_IT(&huart_used, esp_rx_buffer, esp_rx_buffer_size);
}
/*---------Сервисная функция. RxCpltCallback. Переполнение rx буфера---------*/

/*----------Отправка bool в массив-----------------*/
/// Отправка bool в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_bool(uint8_t VARIABLE_ID, bool data, uint8_t *tx_buffer) {
	tx_buffer[0] = 0xFF;
	tx_buffer[1] = VARIABLE_ID;
	tx_buffer[2] = 0x01;
	tx_buffer[3] = 0x0;
	tx_buffer[4] = 0x0;
	tx_buffer[5] = 0x0;
	tx_buffer[6] = data;
	uint16_t CRC_tx_buffer = (~(tx_buffer[1] + tx_buffer[2] + tx_buffer[3] + tx_buffer[4] + tx_buffer[5] + tx_buffer[6])) + 1;
	tx_buffer[7] = CRC_tx_buffer >> 8;
	tx_buffer[8] = CRC_tx_buffer;

	if (huart_used.gState != HAL_UART_STATE_BUSY_TX) {
		HAL_UART_Transmit_IT(&huart_used, tx_buffer, 9);
	}
}
/*----------Отправка bool в массив-----------------*/

/*----------Отправка int8_t в массив-----------------*/
/// Отправка int8_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_int8_t(uint8_t VARIABLE_ID, int8_t data, uint8_t *tx_buffer) {
	tx_buffer[0] = 0xFF; //Start byte
	tx_buffer[1] = VARIABLE_ID;
	tx_buffer[2] = 0x02;
	tx_buffer[3] = data >> 8;
	tx_buffer[4] = data >> 8;
	tx_buffer[5] = data >> 8;
	tx_buffer[6] = data;
	uint16_t CRC_tx_buffer = (~(tx_buffer[1] + tx_buffer[2] + tx_buffer[3] + tx_buffer[4] + tx_buffer[5] + tx_buffer[6])) + 1;
	tx_buffer[7] = CRC_tx_buffer >> 8;
	tx_buffer[8] = CRC_tx_buffer;

	if (huart_used.gState != HAL_UART_STATE_BUSY_TX) {
		HAL_UART_Transmit_IT(&huart_used, tx_buffer, 9);
	}
}
/*----------Отправка int8_t в массив-----------------*/

/*----------Отправка uint8_t в массив-----------------*/
/// Отправка uint8_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_uint8_t(uint8_t VARIABLE_ID, int8_t data, uint8_t *tx_buffer) {
	tx_buffer[0] = 0xFF; //Start byte
	tx_buffer[1] = VARIABLE_ID;
	tx_buffer[2] = 0x03;
	tx_buffer[3] = data >> 8;
	tx_buffer[4] = data >> 8;
	tx_buffer[5] = data >> 8;
	tx_buffer[6] = data;
	uint16_t CRC_tx_buffer = (~(tx_buffer[1] + tx_buffer[2] + tx_buffer[3] + tx_buffer[4] + tx_buffer[5] + tx_buffer[6])) + 1;
	tx_buffer[7] = CRC_tx_buffer >> 8;
	tx_buffer[8] = CRC_tx_buffer;

	if (huart_used.gState != HAL_UART_STATE_BUSY_TX) {
		HAL_UART_Transmit_IT(&huart_used, tx_buffer, 9);
	}
}
/*----------Отправка uint8_t в массив-----------------*/

/*----------Отправка int16_t в массив-----------------*/
/// Отправка int16_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_int16_t(uint8_t VARIABLE_ID, int16_t data, uint8_t *tx_buffer) {
	tx_buffer[0] = 0xFF; //Start byte
	tx_buffer[1] = VARIABLE_ID;
	tx_buffer[2] = 0x04;
	tx_buffer[3] = data >> 16;
	tx_buffer[4] = data >> 16;
	tx_buffer[5] = data >> 8;
	tx_buffer[6] = data;
	uint16_t CRC_tx_buffer = (~(tx_buffer[1] + tx_buffer[2] + tx_buffer[3] + tx_buffer[4] + tx_buffer[5] + tx_buffer[6])) + 1;
	tx_buffer[7] = CRC_tx_buffer >> 8;
	tx_buffer[8] = CRC_tx_buffer;

	if (huart_used.gState != HAL_UART_STATE_BUSY_TX) {
		HAL_UART_Transmit_IT(&huart_used, tx_buffer, 9);
	}
}
/*----------Отправка int16_t в массив-----------------*/

/*----------Отправка uint16_t в массив-----------------*/
/// Отправка uint16_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_uint16_t(uint8_t VARIABLE_ID, uint16_t data, uint8_t *tx_buffer) {
	tx_buffer[0] = 0xFF; //Start byte
	tx_buffer[1] = VARIABLE_ID;
	tx_buffer[2] = 0x05;
	tx_buffer[3] = data >> 16;
	tx_buffer[4] = data >> 16;
	tx_buffer[5] = data >> 8;
	tx_buffer[6] = data;
	uint16_t CRC_tx_buffer = (~(tx_buffer[1] + tx_buffer[2] + tx_buffer[3] + tx_buffer[4] + tx_buffer[5] + tx_buffer[6])) + 1;
	tx_buffer[7] = CRC_tx_buffer >> 8;
	tx_buffer[8] = CRC_tx_buffer;

	if (huart_used.gState != HAL_UART_STATE_BUSY_TX) {
		HAL_UART_Transmit_IT(&huart_used, tx_buffer, 9);
	}
}
/*----------Отправка uint16_t в массив-----------------*/

/*----------Отправка int32_t в массив-----------------*/
/// Отправка int32_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_int32_t(uint8_t VARIABLE_ID, int32_t data, uint8_t *tx_buffer) {
	tx_buffer[0] = 0xFF; //Start byte
	tx_buffer[1] = VARIABLE_ID;
	tx_buffer[2] = 0x06;
	tx_buffer[3] = data >> 24;
	tx_buffer[4] = data >> 16;
	tx_buffer[5] = data >> 8;
	tx_buffer[6] = data;
	uint16_t CRC_tx_buffer = (~(tx_buffer[1] + tx_buffer[2] + tx_buffer[3] + tx_buffer[4] + tx_buffer[5] + tx_buffer[6])) + 1;
	tx_buffer[7] = CRC_tx_buffer >> 8;
	tx_buffer[8] = CRC_tx_buffer;

	if (huart_used.gState != HAL_UART_STATE_BUSY_TX) {
		HAL_UART_Transmit_IT(&huart_used, tx_buffer, 9);
	}
}
/*----------Отправка int32_t в массив-----------------*/

/*----------Отправка uint32_t в массив-----------------*/
void UART_MQTT_Send_data_uint32_t(uint8_t VARIABLE_ID, uint32_t data, uint8_t *tx_buffer) {
	tx_buffer[0] = 0xFF; //Start byte
	tx_buffer[1] = VARIABLE_ID;
	tx_buffer[2] = 0x07;
	tx_buffer[3] = data >> 24;
	tx_buffer[4] = data >> 16;
	tx_buffer[5] = data >> 8;
	tx_buffer[6] = data;
	uint16_t CRC_tx_buffer = (~(tx_buffer[1] + tx_buffer[2] + tx_buffer[3] + tx_buffer[4] + tx_buffer[5] + tx_buffer[6])) + 1;
	tx_buffer[7] = CRC_tx_buffer >> 8;
	tx_buffer[8] = CRC_tx_buffer;

	if (huart_used.gState != HAL_UART_STATE_BUSY_TX) {
		HAL_UART_Transmit_IT(&huart_used, tx_buffer, 9);
	}
}
/*----------Отправка uint32_t в массив-----------------*/

/*----------Отправка float в массив-----------------*/
/// Отправка float в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_float(uint8_t VARIABLE_ID, float data, uint8_t *tx_buffer) {
	union {
		uint32_t res;
		float f;
	} data_conv;
	data_conv.f = data;
	tx_buffer[0] = 0xFF;
	tx_buffer[1] = VARIABLE_ID;
	tx_buffer[2] = 0x08;
	tx_buffer[3] = data_conv.res >> 24;
	tx_buffer[4] = data_conv.res >> 16;
	tx_buffer[5] = data_conv.res >> 8;
	tx_buffer[6] = data_conv.res;
	uint16_t CRC_tx_buffer = (~(tx_buffer[1] + tx_buffer[2] + tx_buffer[3] + tx_buffer[4] + tx_buffer[5] + tx_buffer[6])) + 1;
	tx_buffer[7] = CRC_tx_buffer >> 8;
	tx_buffer[8] = CRC_tx_buffer;

	if (huart_used.gState != HAL_UART_STATE_BUSY_TX) {
		HAL_UART_Transmit_IT(&huart_used, tx_buffer, 9);
	}
}
/*----------Отправка float в массив-----------------*/

/*----------Получить данные bool из массива-----------------*/
/// Получить bool из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
bool UART_MQTT_Receive_data_bool(uint8_t *rx_buffer) {
	bool data = 0x00000000;
	data = rx_buffer[6];
	return data;
}
/*----------Получить данные bool из массива-----------------*/

/*----------Получить данные int8_t из массива-----------------*/
/// Получить int8_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
int8_t UART_MQTT_Receive_data_int8_t(uint8_t *rx_buffer) {
	int8_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 8) | ((data | rx_buffer[4]) << 8) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные int8_t из массива-----------------*/

/*----------Получить данные uint8_t из массива-----------------*/
/// Получить uint8_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
uint8_t UART_MQTT_Receive_data_uint8_t(uint8_t *rx_buffer) {
	uint8_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 8) | ((data | rx_buffer[4]) << 8) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные uint8_t из массива-----------------*/

/*----------Получить данные int16_t из массива-----------------*/
/// Получить int16_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
int16_t UART_MQTT_Receive_data_int16_t(uint8_t *rx_buffer) {
	int16_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 16) | ((data | rx_buffer[4]) << 16) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные int16_t из массива-----------------*/

/*----------Получить данные uint16_t из массива-----------------*/
/// Получить uint16_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
uint16_t UART_MQTT_Receive_data_uint16_t(uint8_t *rx_buffer) {
	uint16_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 16) | ((data | rx_buffer[4]) << 16) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные uint16_t из массива-----------------*/

/*----------Получить данные int32_t из массива-----------------*/
/// Получить int32_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
int32_t UART_MQTT_Receive_data_int32_t(uint8_t *rx_buffer) {
	int32_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 24) | ((data | rx_buffer[4]) << 16) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные int32_t из массива-----------------*/

/*----------Получить данные uint32_t из массива-----------------*/
/// Получить uint32_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
uint32_t UART_MQTT_Receive_data_uint32_t(uint8_t *rx_buffer) {
	uint32_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 24) | ((data | rx_buffer[4]) << 16) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные uint32_t из массива-----------------*/

/*----------Получить данные float из массива-----------------*/
/// Получить float из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
float UART_MQTT_Receive_data_float(uint8_t *rx_buffer) {
	union {
		uint32_t res;
		float f;
	} data_conv;
	uint32_t data = 0x00000000;
	data_conv.res = ((data | rx_buffer[3]) << 24) | ((data | rx_buffer[4]) << 16) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	float VARIABLE_float = data_conv.f;
	return (VARIABLE_float);
}
/*----------Получить данные float из массива-----------------*/

/*----------Проверка контрольной суммы входящего сообщения-----------------*/
/// Проверка контрольной суммы входящего сообщения
/// \param *rx_buffer - буфер, используемый для приема данных
bool UART_MQTT_Checksumm_validation(uint8_t *rx_buffer) {
	bool result;
	uint16_t CRC_rx_buffer = 0x0000;
	uint16_t CRC_check = 0x0000;
	CRC_rx_buffer = (~(rx_buffer[1] + rx_buffer[2] + rx_buffer[3] + rx_buffer[4] + rx_buffer[5] + rx_buffer[6])) + 1;
	CRC_check = (CRC_check | rx_buffer[7] << 8) | (CRC_check | rx_buffer[8]);
	//CRC_rx_buffer == CRC_check ? result = true : result = false;
	if (CRC_rx_buffer == CRC_check) {
		result = true;
	} else {
		result = false;
	}
	return result;
}
/*----------Проверка контрольной суммы входящего сообщения-----------------*/

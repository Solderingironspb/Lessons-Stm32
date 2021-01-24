/*
 * uart_mqtt_lib.c
 *
 *  Created on: 2 сент. 2020 г.
 *      Author: Oleg Volkov
 */

#include "uart_mqtt_lib.h"
extern uint8_t esp_tx_buffer[9];

/*----------Отправка bool в массив-----------------*/
/// Отправка bool в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_bool(uint8_t VARIABLE_ID, bool data, uint8_t *tx_buffer)
{
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

	Serial.write(tx_buffer, 9);
}
/*----------Отправка bool в массив-----------------*/

/*----------Отправка int8_t в массив-----------------*/
/// Отправка int8_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_int8_t(uint8_t VARIABLE_ID, int8_t data, uint8_t *tx_buffer)
{
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

	Serial.write(tx_buffer, 9);
}
/*----------Отправка int8_t в массив-----------------*/

/*----------Отправка uint8_t в массив-----------------*/
/// Отправка uint8_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_uint8_t(uint8_t VARIABLE_ID, int8_t data, uint8_t *tx_buffer)
{
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

	Serial.write(tx_buffer, 9);
}
/*----------Отправка uint8_t в массив-----------------*/

/*----------Отправка int16_t в массив-----------------*/
/// Отправка int16_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_int16_t(uint8_t VARIABLE_ID, int16_t data, uint8_t *tx_buffer)
{
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

	Serial.write(tx_buffer, 9);
}
/*----------Отправка int16_t в массив-----------------*/

/*----------Отправка uint16_t в массив-----------------*/
/// Отправка uint16_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_uint16_t(uint8_t VARIABLE_ID, uint16_t data, uint8_t *tx_buffer)
{
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

	Serial.write(tx_buffer, 9);
}
/*----------Отправка uint16_t в массив-----------------*/

/*----------Отправка int32_t в массив-----------------*/
/// Отправка int32_t в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_int32_t(uint8_t VARIABLE_ID, int32_t data, uint8_t *tx_buffer)
{
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

	Serial.write(tx_buffer, 9);
}
/*----------Отправка int32_t в массив-----------------*/

/*----------Отправка uint32_t в массив-----------------*/
void UART_MQTT_Send_data_uint32_t(uint8_t VARIABLE_ID, uint32_t data, uint8_t *tx_buffer)
{
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

	Serial.write(tx_buffer, 9);
}
/*----------Отправка uint32_t в массив-----------------*/

/*----------Отправка float в массив-----------------*/
/// Отправка float в tx_buffer(принимающему устройству)
/// \param VARIABLE_ID - номер переменной(должен совпадать на передающем и принимающем устройстве)
/// \param data - переменная, которую следует отправить
/// \param *tx_buffer - буфер, который используется для отправки
void UART_MQTT_Send_data_float(uint8_t VARIABLE_ID, float data, uint8_t *tx_buffer)
{
	union
	{
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

	Serial.write(tx_buffer, 9);
}
/*----------Отправка float в массив-----------------*/

/*----------Получить данные bool из массива-----------------*/
/// Получить bool из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
bool UART_MQTT_Receive_data_bool(uint8_t *rx_buffer)
{
	bool data = 0x00000000;
	data = rx_buffer[6];
	return data;
}
/*----------Получить данные bool из массива-----------------*/

/*----------Получить данные int8_t из массива-----------------*/
/// Получить int8_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
int8_t UART_MQTT_Receive_data_int8_t(uint8_t *rx_buffer)
{
	int8_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 8) | ((data | rx_buffer[4]) << 8) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные int8_t из массива-----------------*/

/*----------Получить данные uint8_t из массива-----------------*/
/// Получить uint8_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
uint8_t UART_MQTT_Receive_data_uint8_t(uint8_t *rx_buffer)
{
	uint8_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 8) | ((data | rx_buffer[4]) << 8) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные uint8_t из массива-----------------*/

/*----------Получить данные int16_t из массива-----------------*/
/// Получить int16_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
int16_t UART_MQTT_Receive_data_int16_t(uint8_t *rx_buffer)
{
	int16_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 16) | ((data | rx_buffer[4]) << 16) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные int16_t из массива-----------------*/

/*----------Получить данные uint16_t из массива-----------------*/
/// Получить uint16_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
uint16_t UART_MQTT_Receive_data_uint16_t(uint8_t *rx_buffer)
{
	uint16_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 16) | ((data | rx_buffer[4]) << 16) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные uint16_t из массива-----------------*/

/*----------Получить данные int32_t из массива-----------------*/
/// Получить int32_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
int32_t UART_MQTT_Receive_data_int32_t(uint8_t *rx_buffer)
{
	int32_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 24) | ((data | rx_buffer[4]) << 16) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные int32_t из массива-----------------*/

/*----------Получить данные uint32_t из массива-----------------*/
/// Получить uint32_t из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
uint32_t UART_MQTT_Receive_data_uint32_t(uint8_t *rx_buffer)
{
	uint32_t data = 0x00000000;
	data = ((data | rx_buffer[3]) << 24) | ((data | rx_buffer[4]) << 16) | ((data | rx_buffer[5]) << 8) | ((data | rx_buffer[6]));
	return data;
}
/*----------Получить данные uint32_t из массива-----------------*/

/*----------Получить данные float из массива-----------------*/
/// Получить float из rx_buffer(от передающего устройства)
/// \param *rx_buffer - буфер, используемый для приема данных
float UART_MQTT_Receive_data_float(uint8_t *rx_buffer)
{
	union
	{
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
bool UART_MQTT_Checksumm_validation(uint8_t *rx_buffer)
{
	bool result;
	uint16_t CRC_rx_buffer = 0x0000;
	uint16_t CRC_check = 0x0000;
	CRC_rx_buffer = (~(rx_buffer[1] + rx_buffer[2] + rx_buffer[3] + rx_buffer[4] + rx_buffer[5] + rx_buffer[6])) + 1;
	CRC_check = (CRC_check | rx_buffer[7] << 8) | (CRC_check | rx_buffer[8]);
	//CRC_rx_buffer == CRC_check ? result = true : false;
	if (CRC_rx_buffer == CRC_check)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}
/*----------Проверка контрольной суммы входящего сообщения-----------------*/

/*------------------------служебные сообщения---------------------------------------------*/
void wifi_ok(void)
{
  esp_tx_buffer[0] = 0xFF;
  esp_tx_buffer[1] = 0xFF;
  esp_tx_buffer[2] = 0xFF;
  esp_tx_buffer[3] = 0xAA;
  esp_tx_buffer[4] = 0xAA;
  esp_tx_buffer[5] = 0xAA;
  esp_tx_buffer[6] = 0xAA;
  esp_tx_buffer[7] = 0xFB;
  esp_tx_buffer[8] = 0x5A;
  Serial.write(esp_tx_buffer, 9);
}

void wifi_not_ok(void)
{
  esp_tx_buffer[0] = 0xFF;
  esp_tx_buffer[1] = 0xFF;
  esp_tx_buffer[2] = 0xFF;
  esp_tx_buffer[3] = 0xBB;
  esp_tx_buffer[4] = 0xBB;
  esp_tx_buffer[5] = 0xBB;
  esp_tx_buffer[6] = 0xBB;
  esp_tx_buffer[7] = 0xFB;
  esp_tx_buffer[8] = 0x16;
  Serial.write(esp_tx_buffer, 9);
}

void mqtt_ok(void)
{
  esp_tx_buffer[0] = 0xFF;
  esp_tx_buffer[1] = 0xFF;
  esp_tx_buffer[2] = 0xFF;
  esp_tx_buffer[3] = 0xCC;
  esp_tx_buffer[4] = 0xCC;
  esp_tx_buffer[5] = 0xCC;
  esp_tx_buffer[6] = 0xCC;
  esp_tx_buffer[7] = 0xFA;
  esp_tx_buffer[8] = 0xD2;
  Serial.write(esp_tx_buffer, 9);
}

void mqtt_not_ok(void)
{
  esp_tx_buffer[0] = 0xFF;
  esp_tx_buffer[1] = 0xFF;
  esp_tx_buffer[2] = 0xFF;
  esp_tx_buffer[3] = 0xDD;
  esp_tx_buffer[4] = 0xDD;
  esp_tx_buffer[5] = 0xDD;
  esp_tx_buffer[6] = 0xDD;
  esp_tx_buffer[7] = 0xFA;
  esp_tx_buffer[8] = 0x8E;
  Serial.write(esp_tx_buffer, 9);
}
/*------------------------служебные сообщения---------------------------------------------*/
/*
 * uart_mqtt_lib.h
 *
 *  Created on: 2 сент. 2020 г.
 *      Author: Oleg Volkov
 */

#ifndef SRC_UART_MQTT_LIB_H_
#define SRC_UART_MQTT_LIB_H_

#include <Arduino.h>

void UART_MQTT_Send_data_bool(uint8_t VARIABLE_ID, bool data, uint8_t *tx_buffer);
void UART_MQTT_Send_data_int8_t(uint8_t VARIABLE_ID, int8_t data, uint8_t *tx_buffer);
void UART_MQTT_Send_data_uint8_t(uint8_t VARIABLE_ID, int8_t data, uint8_t *tx_buffer);
void UART_MQTT_Send_data_int16_t(uint8_t VARIABLE_ID, int16_t data, uint8_t *tx_buffer);
void UART_MQTT_Send_data_uint16_t(uint8_t VARIABLE_ID, uint16_t data, uint8_t *tx_buffer);
void UART_MQTT_Send_data_int32_t(uint8_t VARIABLE_ID, int32_t data, uint8_t *tx_buffer);
void UART_MQTT_Send_data_uint32_t(uint8_t VARIABLE_ID, uint32_t data, uint8_t *tx_buffer);
void UART_MQTT_Send_data_float(uint8_t VARIABLE_ID, float data, uint8_t *tx_buffer);
bool UART_MQTT_Receive_data_bool(uint8_t *rx_buffer);
int8_t UART_MQTT_Receive_data_int8_t(uint8_t *rx_buffer);
uint8_t UART_MQTT_Receive_data_uint8_t(uint8_t *rx_buffer);
int16_t UART_MQTT_Receive_data_int16_t(uint8_t *rx_buffer);
uint16_t UART_MQTT_Receive_data_uint16_t(uint8_t *rx_buffer);
int32_t UART_MQTT_Receive_data_int32_t(uint8_t *rx_buffer);
uint32_t UART_MQTT_Receive_data_uint32_t(uint8_t *rx_buffer);
float UART_MQTT_Receive_data_float(uint8_t *rx_buffer);
bool UART_MQTT_Checksumm_validation(uint8_t *rx_buffer);
void wifi_ok(void);
void wifi_not_ok(void);
void mqtt_ok(void);
void mqtt_not_ok(void);


#endif /* SRC_UART_MQTT_LIB_H_ */

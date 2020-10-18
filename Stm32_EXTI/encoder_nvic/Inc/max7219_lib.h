/*
 * max7219_lib.h
 *
 *  Created on: Sep 27, 2020
 *      Author: Oleg Volkov
 */

#ifndef INC_MAX7219_LIB_H_
#define INC_MAX7219_LIB_H_

#include <main.h>
#include <stdbool.h>

void max7219_Transmit(uint8_t adress, uint8_t data);
void max7219_Shutdown(bool data);
void max7219_Init(void);
void max7219_Display_Test(bool data);
void max7219_Display_Clean(void);
void max7219_Intensity(uint8_t data);
void max7219_send_symbol(uint8_t *tx_buffer);
void max7219_num(uint8_t rg, uint32_t value);
void max7219_num_point(uint8_t rg, uint32_t value);
void max7219_Send_int32_t(int32_t value);
void max7219_Send_float(float value);

#endif /* INC_MAX7219_LIB_H_ */

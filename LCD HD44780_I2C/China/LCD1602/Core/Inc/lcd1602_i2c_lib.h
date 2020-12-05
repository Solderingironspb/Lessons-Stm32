/*
 * lcd1602_i2c_lib.h
 *
 *  Библиотека для китайских дисплеев
 *  Created on: Nov 24, 2020
 *      Authors: Oleg Volkov & Konstantin Golinskiy
 *      Создать свой символ: https://www.quinapalus.com/hd44780udg.html
 */

#ifndef INC_LCD1602_I2C_LIB_H_
#define INC_LCD1602_I2C_LIB_H_

#include <main.h>
#include <string.h>
#include <stdbool.h>

void lcd1602_Backlight(bool state);
void lcd1602_Init(void);
void lcd1602_Clean(void);
void lcd1602_SetCursor(uint8_t x, uint8_t y);
void lcd1602_Print_symbol(uint8_t symbol);
void lcd1602_Print_text(char *message);
void lcd1602_Move_to_the_left(void);
void lcd1602_Move_to_the_right(void);
void lcd1602_Create_symbol( uint8_t *my_symbol, uint8_t memory_adress);
void lcd1602_Clean_Text(void);

#endif /* INC_LCD1602_I2C_LIB_H_ */

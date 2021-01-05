/*
 * ST7920_lib.h
 *
 *  Created on: 10 дек. 2020 г.
 *      Author: Oleg Volkov, Konstantin Golinsky
 *
 *  YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
 *  GitHub: https://github.com/Solderingironspb/Lessons-Stm32/blob/master/README.md
 *  Группа ВК: https://vk.com/solderingiron.stm32
 *
 *  Кодировка UTF-8 Basic Latin: https://www.utf8-chartable.de/unicode-utf8-table.pl
 *  Кодировка UTF-8 Cyrillic: https://www.utf8-chartable.de/unicode-utf8-table.pl?start=1024&names=-&unicodeinhtml=hex
 *  Программа для конвертации изображения.bmp в bitmap: http://en.radzio.dxp.pl/bitmap_converter/
 */

#ifndef INC_ST7920_LIB_H_
#define INC_ST7920_LIB_H_


#include <main.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*================= Демонстрационное лого. Можно вырезать. =====================*/
void ST7920_logo_demonstration(void);
/*================= Демонстрационное лого. Можно вырезать. =====================*/

void ST7920_Init(void);
void ST7920_Send_symbol_text(uint8_t y, uint8_t x, char *message);
void ST7920_Graphic_mode(bool enable);
void ST7920_Clean();
void ST7920_Clean_Frame_buffer(void);
void ST7920_Draw_bitmap(const unsigned char *bitmap);
void ST7920_Draw_pixel(uint8_t x, uint8_t y);
void ST7920_Update(void);
void ST7920_Clean_pixel(uint8_t x, uint8_t y);
void ST7920_Decode_UTF8(uint16_t x, uint8_t y, bool inversion, char *tx_buffer);
void ST7920_Inversion(uint16_t x_start, uint16_t x_end);
void ST7920_Draw_line(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end);
void ST7920_Draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void ST7920_Draw_rectangle_filled(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void ST7920_Draw_circle(uint8_t x, uint8_t y, uint8_t radius);
void ST7920_Draw_circle_filled(int16_t x, int16_t y, int16_t radius);
void ST7920_Draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
void ST7920_Draw_triangle_filled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);


#endif /* INC_ST7920_LIB_H_ */

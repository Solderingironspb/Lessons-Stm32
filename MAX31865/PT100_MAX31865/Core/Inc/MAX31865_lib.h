/*
 * MAX31865_lib.h
 *
 *  Created on: 7 июн. 2021 г.
 *      Author: Волков Олег
 *
 *  YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
 *  GitHub: https://github.com/Solderingironspb/Lessons-Stm32/blob/master/README.md
 *  Группа ВК: https://vk.com/solderingiron.stm32
 *
 *  Расчет температуры PT100, в зависимости от сопротивления: здесь будет ссылка на мой расчет.
 *  MAX31865 datasheet: https://datasheets.maximintegrated.com/en/ds/MAX31865.pdf
 */

#ifndef INC_MAX31865_LIB_H_
#define INC_MAX31865_LIB_H_

#include <main.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

void MAX31865_Init(uint8_t num_wires);
uint8_t MAX31865_Configuration_info(void);
double MAX31865_Get_Temperature(void);
double MAX31865_Get_Temperature_math(double PT100_Resistance);

#endif /* INC_MAX31865_LIB_H_ */

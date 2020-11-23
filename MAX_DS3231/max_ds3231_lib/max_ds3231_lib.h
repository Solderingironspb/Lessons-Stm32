/*
 * max_ds3231_lib.h
 *
 *  Created on: 14 нояб. 2020 г.
 *      Author: Oleg Volkov
 */

#ifndef MAX_DS3231_LIB_H_
#define MAX_DS3231_LIB_H_

#include <main.h>
#include <stdio.h>

void max_ds3231_get_time(void);
void max_ds3231_get_temperature(void);
void max_ds3231_set_seconds(uint8_t seconds);
void max_ds3231_set_minutes(uint8_t minutes);
void max_ds3231_set_hours(uint8_t hours);
void max_ds3231_set_day(uint8_t day);
void max_ds3231_set_date(uint8_t date);
void max_ds3231_set_month_cuntury(uint8_t month, uint8_t cuntury);
void max_ds3231_set_year(uint16_t year);


#endif /* MAX_DS3231_LIB_H_ */

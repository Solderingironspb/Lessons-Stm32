/*
 * MAX31865_lib.c
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

#include "MAX31865_lib.h"

/*--------------Характеристики датчика типа PT100 и референсный резистор, подключенный к MAX31865------------*/
#define MAX31865_PT100_R0 (double)100.0 //Сопротивление датчика PT100, при 0 °С
#define MAX31865_Rref (double)428.5 //Сопротивление референсного резистора, подключенного к MAX31865
/*--------------Характеристики датчика типа PT100 и референсный резистор, подключенный к MAX31865------------*/

/*-----------Коэффициенты из ГОСТ 6651-2009 для датчика типа PT100(Платиновые ТС И ЧЭ, 0.00385°С^-1)---------*/
#define MAX31865_A (double)0.0039083
#define MAX31865_B (double)0.0000005775
/*-----------Коэффициенты из ГОСТ 6651-2009 для датчика типа PT100(Платиновые ТС И ЧЭ, 0.00385°С^-1)---------*/

/*-----------------------------------------Глобальные переменные---------------------------------------------*/
double MAX31865_PT100_R = 0.0; //Глобальная переменная, определяющая сопротивление датчика PT100
double MAX31865_PT100_T = 0.0; //Глобальная переменная, определяющая температуру датчика PT100
bool MAX31865_Sensor_Error = 0; //Глобальная переменная, определяющая неисправность датчика PT100
/*-----------------------------------------Глобальные переменные---------------------------------------------*/

/*-------------------------------------------Для работы по spi-----------------------------------------------*/
#define cs_set() CS_GPIO_Port-> BSRR = (uint32_t) CS_Pin << 16u; //CS_притягиваем к земле
#define cs_reset() CS_GPIO_Port ->BSRR = CS_Pin; //CS притягиваем к 3.3v
extern SPI_HandleTypeDef hspi1;
//P.S. Максимальная скорость spi 5 МГц.
//Также обратите внимание, что Clock Polarity(CLPOL) = Low. Clock Phase(CPHA) = 2 Edge.
/*-------------------------------------------Для работы по spi-----------------------------------------------*/

/*=======================ИНИЦИАЛИЗАЦИЯ МОДУЛЯ MAX31865=========================*/
void MAX31865_Init(uint8_t num_wires) {
	///Функция инициализация модуля MAX31865
	///Не вижу особого смысла выводить полную настройку модуля, поэтому сделаем
	///небольшое упрощение для конечного пользователя
	///все, что может настроить пользователь - это выбрать тип подключения
	///2,3 или 4 проводное
	/// \param num_wires - тип подключения датчика 2,3 или 4 проводное
	uint8_t MAX31865_Reinitialization_cnt = 0;
	MAX31865_Sensor_Error = 0;
	uint8_t MAX31865_Configuration_register_write[] = { 0x80, 0x00 };
	if (num_wires == 2 || num_wires == 4) {
		MAX31865_Configuration_register_write[1] = 0xC3; //0xC3
	} else if (num_wires == 3) {
		MAX31865_Configuration_register_write[1] = 0xD3; //0xD3
	}
	cs_set();
	HAL_SPI_Transmit(&hspi1, MAX31865_Configuration_register_write, 2, 100);
	cs_reset();
	//Дабы достучаться до датчика после подачи питания, т.к. с первого раза инициализация может и не пройти, заведем цикл.
	while (MAX31865_Configuration_info() != 0xD1 || MAX31865_Configuration_info() != 0xC1) {
		MAX31865_Reinitialization_cnt++;

		cs_set();
		HAL_SPI_Transmit(&hspi1, MAX31865_Configuration_register_write, 2, 100);
		cs_reset();

		if (MAX31865_Reinitialization_cnt == 100) {
			//printf("Initialization MAX31865 != OK\r\n");
			break;
		}

	}

}
/*=======================ИНИЦИАЛИЗАЦИЯ МОДУЛЯ MAX31865=========================*/

/*====================ИНФОРМАЦИЯ О НАСТРОЙКЕ МОДУЛЯ MAX31865===================*/
uint8_t MAX31865_Configuration_info(void) {
	///Функция получения информации о конфигурации модуля MAX31865
	///Возвращает значение конфигурации.
	///Не удивляйтесь, если отправите при инициализации 0xC3, а получите 0xC1
	///(см. datasheet MAX31865 стр.14 "The fault status clear bit D1, self-clears to 0.")
	uint8_t read_data = 0x00;
	uint8_t MAX31865_Configuration = 0x00;
	cs_set();
	HAL_SPI_Transmit(&hspi1, &read_data, 1, 100);
	HAL_SPI_Receive(&hspi1, &MAX31865_Configuration, 1, 100);
	cs_reset();
	return MAX31865_Configuration;
}
/*====================ИНФОРМАЦИЯ О НАСТРОЙКЕ МОДУЛЯ MAX31865===================*/

/*===================ОСНОВНАЯ ФУНКЦИЯ РАБОТЫ С МОДУЛЕМ MAX31865==================*/
double MAX31865_Get_Temperature(void) {
	///Основная функция работы с модулем MAX31865
	///Просходит обращение к начальному адресу регистра памяти модуля и из него читаем 7 байт.
	///В функцию также включена самодиагностика модуля, которая сообщит, если с датчиком будет что-то не так.

	double data; //переменная для вычислений

	struct rx_data_MAX31865 {
		uint16_t RTD_Resistance_Registers; //Регистры сопротивления
		uint16_t High_Fault_Threshold; //Верхний порог неисправности
		uint16_t Low_Fault_Threshold; //Нижний порог неисправности
		uint8_t Fault_Status; //Статус неисправности
	};

	struct rx_data_MAX31865 MAX31865_receieve_data;

	uint8_t MAX31865_start_address_of_the_poll = 0x01; //Адрес регистра, с которого начнем чтение данных
	uint8_t MAX31865_rx_buffer[7]; //буфер, куда будем складывать приходящие данные
	cs_set();
	HAL_SPI_Transmit(&hspi1, &MAX31865_start_address_of_the_poll, 1, 100);
	HAL_SPI_Receive(&hspi1, MAX31865_rx_buffer, 7, 100);
	cs_reset();
	MAX31865_receieve_data.RTD_Resistance_Registers = ((MAX31865_rx_buffer[0] << 8) | MAX31865_rx_buffer[1]) >> 1; //Данные регистров сопротивления
	MAX31865_receieve_data.High_Fault_Threshold = ((MAX31865_rx_buffer[2] << 8) | MAX31865_rx_buffer[3]) >> 1; //Данные верхнего порого неисправности
	MAX31865_receieve_data.Low_Fault_Threshold = (MAX31865_rx_buffer[4] << 8) | MAX31865_rx_buffer[5]; //Данные нижнего порога неисправности
	MAX31865_receieve_data.Fault_Status = MAX31865_rx_buffer[6]; //Статус неисправности
	if (MAX31865_receieve_data.Fault_Status > 0x00) {

		/*--------------Здесь Ваши действия по реагированию на ошибку датчика---------------*/
		MAX31865_Sensor_Error = 1;
		//printf("Sensor Error!\r\n");

		/*----Автоматический сброс ошибки----*/
		MAX31865_Init(3);
		MAX31865_Sensor_Error = 0;
		/*----Автоматический сброс ошибки----*/

		//Так можно сбросить ошибку, проинициализировав датчик заново.
		//Сброс ошибки, по желанию. Обычно ее не сбрасывают в автомате, а зовут оператора, чтоб квитировал ошибку.
		//До прихода оператора, установка находится в ошибке, все управляющие узлы должны отключаться.
		/*--------------Здесь Ваши действия по реагированию на ошибку датчика---------------*/

	}

	//printf("RTD Resistance Registers = %X\r\n", MAX31865_receieve_data.RTD_Resistance_Registers);
	//printf("Hight Fault Treshold = %X\r\n", MAX31865_receieve_data.High_Fault_Threshold);
	//printf("Low Fault Treshold = %X\r\n", MAX31865_receieve_data.Low_Fault_Threshold);
	//printf("Fault status = %X\r\n", MAX31865_receieve_data.Fault_Status = MAX31865_rx_buffer[6]);
	data = ((double) MAX31865_receieve_data.RTD_Resistance_Registers * MAX31865_Rref ) / (double) 32768.0; // Replace 4000 by 400 for PT100
	//printf("Rrtd = %lf\n", data);
	return MAX31865_Get_Temperature_math(data);
}
/*===================ОСНОВНАЯ ФУНКЦИЯ РАБОТЫ С МОДУЛЕМ MAX31865==================*/

/*===============================================ПРЕОБРАЗОВАНИЕ СОПРОТИВЛЕНИЯ В ТЕМПЕРАТУРУ, СОГЛАСНО ГОСТ 6651-2009===============================================*/
double MAX31865_Get_Temperature_math(double PT100_Resistance) {
	if (PT100_Resistance >= (double) 100.0) {
		double MAX31865_math_Discriminant = (double) 0.00001527480889 - ((double) -0.00000231 * (1 - (PT100_Resistance / MAX31865_PT100_R0 )));
		MAX31865_PT100_T = ((double) -0.0039083 + sqrt(MAX31865_math_Discriminant)) / (double) -0.000001155;
	} else {
		MAX31865_PT100_T = (double) 0.000000000270 * pow(PT100_Resistance, 5) - (double) 0.000000066245 * pow(PT100_Resistance, 4) - (double) 0.000000184636 * pow(PT100_Resistance, 3)
				+ (double) 0.002320232987 * pow(PT100_Resistance, 2) + (double) 2.229927824035 * PT100_Resistance - (double) 242.090854986215;
	}
	return MAX31865_PT100_T;
}
/*===============================================ПРЕОБРАЗОВАНИЕ СОПРОТИВЛЕНИЯ В ТЕМПЕРАТУРУ, СОГЛАСНО ГОСТ 6651-2009===============================================*/

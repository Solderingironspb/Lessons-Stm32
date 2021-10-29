/*
 *  AT24Cxx.с
 *
 *  Библиотека для работы с микросхемой памяти EEPROM типа AT24Cxx
 *  Created on: Oct 29, 2021
 *  Authors: Oleg Volkov & Konstantin Golinskiy
 *  YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
 *  GitHub: https://github.com/Solderingironspb/Lessons-Stm32/blob/master/README.md
 *  GitHub Константина: https://github.com/GolinskiyKonstantin
 *  Группа ВК: https://vk.com/solderingiron.stm32
 *  e-mail: golinskiy.konstantin@gmail.com
 */

#include "AT24Cxx.h"

uint8_t data_buff[AT24CXX_PAGE_BYTE];

/*------------------------------------Проверка наличия микросхемы памяти EEPROM на шине i2c-------------------------------------------------*/
bool AT24xx_Connect_test(void) {
	///Функция проверки наличия микросхемы памяти EEPROM на шине i2c
	///Вернет true, если по указанному адресу i2c, микросхема даст ответ. false при неудаче.

	WP_PORT->BSRR = WP_Pin; //Защита от записи включена
	if (HAL_I2C_IsDeviceReady(&AT24CXX_I2C, AT24CXX_I2C_ADDR, 5, 100) == HAL_OK)
		return true;
	else
		return false;
}
/*------------------------------------Проверка наличия микросхемы памяти EEPROM на шине i2c-------------------------------------------------*/

/*------------------------------------------------Форматирование EEPROM---------------------------------------------------------------------*/
void AT24Cxx_erase_chip(void) {
	///Функция форматирования микросхемы памяти EEPROM

	WP_PORT->BSRR = (uint32_t) WP_Pin << 16u; //Защита от записи отключена
	memset(data_buff, 0xFF, AT24CXX_PAGE_BYTE);
	for (uint16_t i = 0; i < AT24CXX_MAX_MEM_ADDRESS; i = i + AT24CXX_PAGE_BYTE) {

#if defined(AT24C01) || defined(AT24C02)
			HAL_I2C_Mem_Write( &AT24CXX_I2C,  AT24CXX_I2C_ADDR, i, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data_buff, AT24CXX_PAGE_BYTE, HAL_MAX_DELAY );
		#else
		HAL_I2C_Mem_Write(&AT24CXX_I2C, AT24CXX_I2C_ADDR, i, I2C_MEMADD_SIZE_16BIT, (uint8_t*) data_buff, AT24CXX_PAGE_BYTE, HAL_MAX_DELAY);
#endif

		HAL_Delay(10);
	}
	WP_PORT->BSRR = WP_Pin; //Защита от записи включена
}
/*------------------------------------------------Форматирование EEPROM---------------------------------------------------------------------*/

/*------------------------------------Функция записи в память массива данных(uint8_t)-------------------------------------------------------*/
uint16_t AT24Cxx_write(uint16_t addMem_write, uint8_t *data_write, uint16_t size_write) {
	///Функция записи в память массива данных(uint8_t)
	///вернет 0, если запись не произошла ( передали адрес и размер массива больше чем память чипа )
	///если запись удалась, то вернет адрес, на котором зокончили запись. Это удобно для последуещей записи с конца.
	///param addMem_write - адрес памяти, с которого начнем писать данные. Значение от 0 до AT24CXX_MAX_MEM_ADDRESS
	///param data_write - данные, которые хотим записать
	///param size_write - размер массива данных, которые хотим записать

	if ((addMem_write + size_write) < AT24CXX_MAX_MEM_ADDRESS) {
		WP_PORT->BSRR = (uint32_t) WP_Pin << 16u;	//Защита от записи отключена
		uint16_t page_count_write = addMem_write / AT24CXX_PAGE_BYTE;	//узнаем на какой странице мы находимся
		uint16_t byte_count_write = AT24CXX_PAGE_BYTE - (addMem_write - (page_count_write * AT24CXX_PAGE_BYTE));// узнаем сколько байт нужно отправить до следуещей страницы
		if (byte_count_write >= size_write) {	//если размер данных помещается в остаток до конца страницы
			memcpy(data_buff, data_write, size_write);

#if defined(AT24C01) || defined(AT24C02)
				HAL_I2C_Mem_Write( &AT24CXX_I2C,  AT24CXX_I2C_ADDR, addMem_write, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data_buff, size_write, HAL_MAX_DELAY );
			#else
			HAL_I2C_Mem_Write(&AT24CXX_I2C, AT24CXX_I2C_ADDR, addMem_write, I2C_MEMADD_SIZE_16BIT, (uint8_t*) data_buff, size_write, HAL_MAX_DELAY);
#endif

			HAL_Delay(10);
		} else {	//если размер данных не помещается в остаток до конца страницы
			memcpy(data_buff, data_write, byte_count_write);

#if defined(AT24C01) || defined(AT24C02)
				HAL_I2C_Mem_Write( &AT24CXX_I2C,  AT24CXX_I2C_ADDR, addMem_write, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data_buff, byte_count_write, HAL_MAX_DELAY );
			#else
			HAL_I2C_Mem_Write(&AT24CXX_I2C, AT24CXX_I2C_ADDR, addMem_write, I2C_MEMADD_SIZE_16BIT, (uint8_t*) data_buff, byte_count_write,
			HAL_MAX_DELAY);
#endif

			HAL_Delay(10);
			size_write = size_write - byte_count_write;
			addMem_write = addMem_write + byte_count_write;
			uint16_t data_offset_write = byte_count_write;
			while (size_write >= AT24CXX_PAGE_BYTE) {	//если остаток не помещается до конца страницы ( размер на больше чем страница
				memcpy(data_buff, data_write + data_offset_write, AT24CXX_PAGE_BYTE);

#if defined(AT24C01) || defined(AT24C02)
					HAL_I2C_Mem_Write( &AT24CXX_I2C,  AT24CXX_I2C_ADDR, addMem_write, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data_buff, AT24CXX_PAGE_BYTE, HAL_MAX_DELAY );
				#else
				HAL_I2C_Mem_Write(&AT24CXX_I2C, AT24CXX_I2C_ADDR, addMem_write, I2C_MEMADD_SIZE_16BIT, (uint8_t*) data_buff, AT24CXX_PAGE_BYTE,
				HAL_MAX_DELAY);
#endif

				HAL_Delay(10);

				size_write = size_write - AT24CXX_PAGE_BYTE;
				addMem_write = addMem_write + AT24CXX_PAGE_BYTE;
				data_offset_write = data_offset_write + AT24CXX_PAGE_BYTE;

			}
			if (size_write) {

				memcpy(data_buff, data_write + data_offset_write, size_write);

#if defined(AT24C01) || defined(AT24C02)
					HAL_I2C_Mem_Write( &AT24CXX_I2C,  AT24CXX_I2C_ADDR, addMem_write, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data_buff, size_write, HAL_MAX_DELAY );
				#else
				HAL_I2C_Mem_Write(&AT24CXX_I2C, AT24CXX_I2C_ADDR, addMem_write, I2C_MEMADD_SIZE_16BIT, (uint8_t*) data_buff, size_write,
				HAL_MAX_DELAY);
#endif
				HAL_Delay(10);
			}
		}
		WP_PORT->BSRR = WP_Pin; //Защита от записи включена
		return addMem_write + size_write;
	} else {
		return 0;
	}
}
/*------------------------------------Функция записи в память массива данных(uint8_t)-------------------------------------------------------*/

/*------------------------------------Функция чтения из памяти массива данных(uint8_t)------------------------------------------------------*/
uint16_t AT24Cxx_read(uint16_t addMem_read, uint8_t *data_read, uint16_t size_read) {
	///Функция чтения из памяти массива данных(uint8_t)
	///вернет 0, если чтение не произошло ( передали адрес и размер массива больше чем память чипа )
	///если чтение прошло успешно, то вернет адрес, на котором зокончилось чтение. Это удобно для последующего чтения с конца.
	///param addMem_read - адрес памяти, с которого начнем читать данные. Значение от 0 до AT24CXX_MAX_MEM_ADDRESS
	///param data_write - данные, куда запишем считанные данные
	///param size_write - размер массива данных, которые хотим считать

	WP_PORT->BSRR = WP_Pin; //Защита от записи включена
	if ((addMem_read + size_read) < AT24CXX_MAX_MEM_ADDRESS) {

#if defined(AT24C01) || defined(AT24C02)
			HAL_I2C_Mem_Read( &AT24CXX_I2C,  AT24CXX_I2C_ADDR, addMem_read, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data_read, size_read, HAL_MAX_DELAY );
		#else
		HAL_I2C_Mem_Read(&AT24CXX_I2C, AT24CXX_I2C_ADDR, addMem_read, I2C_MEMADD_SIZE_16BIT, (uint8_t*) data_read, size_read, HAL_MAX_DELAY);
#endif

		return addMem_read + size_read;
	} else {
		return 0;
	}
}
/*------------------------------------Функция чтения из памяти массива данных(uint8_t)------------------------------------------------------*/

/*---------------------------------------Функция записи данных в EEPROM с проверкой CRC32---------------------------------------------------*/
bool AT24Cxx_write_data(uint16_t addMem_write, uint8_t *data) {
	///Функция записи данных в EEPROM с проверкой CRC32(в CubeMX нужно активировать модуль CRC)
	///Пример записи функции: AT24Cxx_write_data(0, (uint8_t*) &Temperature_min);
	///Вроде как должно работать на STM32 серии F0, F1, F2, F3, F4, L1.
	///param addMem_write - адрес памяти, с которого начнем писать данные. Данные + CRC32 = 8 байт. Т.е. 1 переменную пишем на адрес 0, вторую на адрес 8, третью на адрес 16 и т.д.
	///param data - данные, которые будем писать.
	///Функция возвращает true при успешной записи в адрес памяти и false при ошибке записи
	uint32_t CRC32 = 0;
	uint32_t data_check = 0;
	uint32_t CRC_check = 0;
	CRC32 = HAL_CRC_Calculate(&AT24CXX_CRC, (uint32_t*) data, 1);
	AT24Cxx_write(addMem_write, (uint8_t*) data, 4); //Данные
	AT24Cxx_write(addMem_write + 4, (uint8_t*) &CRC32, 4); //CRC32
	AT24Cxx_read(addMem_write, (uint8_t*) &data_check, 4); //Данные
	AT24Cxx_read(addMem_write + 4, (uint8_t*) &CRC_check, 4); //CRC32
	if (*(uint32_t*) data == data_check && CRC32 == CRC_check) { //Если отправленные данные равны принятым, то прожиг прошел успешно!
		return true;
	} else {
		return false;
	}
}
/*---------------------------------------Функция записи данных в EEPROM с проверкой CRC32---------------------------------------------------*/

/*---------------------------------------Функция чтения данных с EEPROM с проверкой CRC32---------------------------------------------------*/
bool AT24Cxx_read_data(uint16_t addMem_read, uint8_t *data) {
	///Функция чтения данных с EEPROM с проверкой CRC32(в CubeMX нужно активировать модуль CRC)
	///Пример записи функции: AT24Cxx_read_data(0, (uint8_t*) &Temperature_min);
	///Вроде как должно работать на STM32 серии F0, F1, F2, F3, F4, L1.
	///param addMem_read - адрес памяти, с которого начнем читать данные. Данные + CRC32 = 8 байт.
	///Вы должны знать таблицу адресов, куда писались данные. Если писали, как в примере функции bool AT24Cxx_write_data(uint16_t addMem_write, uint8_t *data),
	///Т.е. 1 переменную писали на адрес 0, вторую на адрес 8, третью на адрес 16 и т.д. То с этих адресов и читаем.
	///param data - Считанные данные, которые поместим в какую-то переменную.
	///Функция возвращает true, если считалось верно и CRC32 данных бьется с CRC32 записанной в EEPROM. Соответственно false при неудачном считывании, либо битых данных.
	uint32_t CRC_check = 0;
	AT24Cxx_read(addMem_read, data, 4); //Данные
	AT24Cxx_read(addMem_read + 4, (uint8_t*) &CRC_check, 4); //CRC32
	if (HAL_CRC_Calculate(&AT24CXX_CRC, (uint32_t*) data, 1) == CRC_check) { //Если CRC принятых данных равна CRC, которые были записаны - значит данные не битые!
		return true;
	} else {
		return false;
	}
}
/*---------------------------------------Функция чтения данных с EEPROM с проверкой CRC32---------------------------------------------------*/

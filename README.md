# STM32. CRC. Теория. Как считать. Алгоритм. CRC HAL. Аппаратный блок. CRC-8/16/32 бита.
![maxresdefault](https://user-images.githubusercontent.com/68805120/151665953-b8664685-fbcd-422c-afee-7f121edb577a.jpg)
Видео: [STM32. CRC. Теория. Как считать. Алгоритм. CRC HAL. Аппаратный блок. CRC-8/16/32 бита.](https://youtu.be/YyQonUJrBn4)
###
```
/*---------------------------Программный расчет CRC-32, как в Stm32--------------------------*/
uint32_t CRC_Calculate_software(uint32_t *Data, uint32_t Buffer_lenght) {
	uint32_t CRC32 = 0xFFFFFFFF;
	while (Buffer_lenght--) {
		CRC32 ^= *Data++;
		for (uint8_t i = 0; i < (sizeof(*Data) * 8); i++) {
			if (CRC32 & 0x80000000) {
				CRC32 = (CRC32 << 1u) ^ 0x04C11DB7;
			} else {
				CRC32 = (CRC32 << 1u);
			}
		}
	}
	return CRC32;
}
/*---------------------------Программный расчет CRC-32, как в Stm32--------------------------*/
```

```
/*------------------------------CRC16(ModbusRTU)----------------------------------*/
uint16_t ModbusRTU_CRC16_Calculate(uint8_t *data, uint8_t lenght, uint8_t byte_order) {
///Функция рассчета CRC16.
/// \param *data - массив данных
///	\param lenght - длина массива данных
/// \param byte_order - порядок байт:
///0 - 1234(младшим регистром вперед, младшим байтом вперед),
///1 - 3412(старшим регистром вперед, младшим байтом вперед),
///2 - 2143(младшим регистром вперед, старшим байтом вперед),
///3 - 4321(старшим регистром вперед, старшим байтом вперед).

	uint16_t crc = 0xFFFF;
	while (lenght--) {
		crc ^= *data++;
		for (int i = 0; i < 8; i++) {
			if (crc & 0x01) {
				crc = (crc >> 1u) ^ 0xA001;
			} else {
				crc = crc >> 1u;
			}
		}
	}
	switch (byte_order) {
	case (0): //1234(младшим регистром вперед, младшим байтом вперед),
		break;
	case (1): //3412(старшим регистром вперед, младшим байтом вперед),
		crc = (crc << 8u) | (crc >> 8u);
		break;
	case (2): //2143(младшим регистром вперед, старшим байтом вперед),
		crc = (((crc >> 8u) & 0x0F) << 12u) | ((crc >> 12u) << 8u) | ((crc << 12u) << 4u) | ((crc >> 4u) & 0x00F);
		break;
	case (3): //4321(старшим регистром вперед, старшим байтом вперед).
		crc = (((crc >> 8u) & 0x0F) << 4u) | (crc >> 12u) | ((crc << 12u) << 12u) | (((crc >> 4u) & 0x00F) << 8u);
		break;
	}
	return crc;
}
/*------------------------------CRC16(ModbusRTU)----------------------------------*/
```
```
/*-------Пример настройки аппаратного модуля Stm32 для расчета CRC-16(Modbus RTU)--------*/
  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
  hcrc.Init.GeneratingPolynomial = 0x8005;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_BYTE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */
/*-------Пример настройки аппаратного модуля Stm32 для расчета CRC-16(Modbus RTU)--------*/
```

Материалы из видео **[[Скачать]](https://github.com/Solderingironspb/Lessons-Stm32/archive/Lesson_pointers_in_C.zip)**
**[[Главная страница]](https://github.com/Solderingironspb/Lessons-Stm32/blob/master/README.md)**

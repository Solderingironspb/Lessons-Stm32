/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
#include "lcd1602_i2c_lib.h"

/*----Русский алфавит----*/
/*Чего нет, то заменяется латинскими буквами*/

#define RU_B 0xA0  //Б
#define RU_G 0xA1  //Г
#define RU_D 0xE0  //Д
#define RU_Eo 0xA2 //Ё
#define RU_Zh 0xA3 //Ж
#define RU_Z 0xA4  //З
#define RU_I 0xA5 //И
#define RU_J 0xA6 //Й
#define RU_L 0xA7 //Л
#define RU_P 0xA8 //П
#define RU_U 0xA9 //У
#define RU_F 0xAA //Ф
#define RU_Ts 0xD6 //Ц
#define RU_Ch 0xAB //Ч
#define RU_Sh 0xAC //Ш
#define RU_Shch 0xE2 //Щ
#define RU_TZ 0xAD //Ъ
#define RU_Y 0xAE //Ы
#define RU_MZ 0x62 //Ь
#define RU_EE 0xAF //Э
#define RU_Yu 0xB0 //Ю
#define RU_Ya 0xB1 //Я

/*Чего нет, то заменяется латинскими буквами*/

#define RU_b 0xB2 //б
#define RU_v 0xB3 //в
#define RU_g 0xB4 //г
#define RU_d 0xE3 //д
#define RU_eo 0xB5 //ё
#define RU_zh 0xB6 //ж
#define RU_z 0xB7 //з
#define RU_i 0xB8 //и
#define RU_j 0xB9 //й
#define RU_k 0xBA //к
#define RU_l 0xBB //л
#define RU_m 0xBC //м
#define RU_n 0xBD //н
#define RU_p 0xBE //п
#define RU_t 0xBF //т
#define RU_f 0xAA //ф
#define RU_ts 0xE5 //ц
#define RU_ch 0xC0 //ч
#define RU_sh 0xC1 //ш
#define RU_shch 0xE6 //щ
#define RU_tz 0xC2 //ъ
#define RU_y 0xC3 //ы
#define RU_mz 0xC4 //ь
#define RU_ee 0xC5 //э
#define RU_yu 0xC6 //ю
#define RU_ya 0xC7 //я

/*----Русский алфавит----*/

extern char lcd1602_tx_buffer[40];
uint8_t flag = 0;
uint8_t procent;
int counter = -56421;
uint32_t Delay = 100;
float counter_float = 0.0f;

uint16_t i = 0;
unsigned long T1, T2;

/*-------------------------------Свои символы---------------------------------*/

const uint8_t symbol_celsius[8] = {
		0x07,	// 00000111
		0x05,	// 00000101
		0x07,	// 00000111
		0x00,	// 00000000
		0x00,	// 00000000
		0x00,	// 00000000
		0x00,	// 00000000
		0x00	// 00000000
		};

/*-------------------------------Свои символы---------------------------------*/

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*------------------------------Отладка по SWO--------------------------------*/
int _write(int file, uint8_t *ptr, int len) {

	for (int DataIdx = 0; DataIdx < len; DataIdx++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}
/*------------------------------Отладка по SWO--------------------------------*/

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */
	HAL_Delay(100);
	lcd1602_Init();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (HAL_GetTick() - T1 >= Delay) {
			T1 = HAL_GetTick();
			if (flag == 0) {
				lcd1602_SetCursor(0, 0);
				if (procent < 25) {
					sprintf(lcd1602_tx_buffer, "%ca%cpy%c%ca                          ", RU_Z, RU_g, RU_z, RU_k);
					lcd1602_Print_text(lcd1602_tx_buffer);
				} else if (procent >= 25 && procent <= 50) {
					sprintf(lcd1602_tx_buffer, "%ca%cpy%c%ca.                         ", RU_Z, RU_g, RU_z, RU_k);
					lcd1602_Print_text(lcd1602_tx_buffer);
				} else if (procent >= 50 && procent <= 75) {
					sprintf(lcd1602_tx_buffer, "%ca%cpy%c%ca..                        ", RU_Z, RU_g, RU_z, RU_k);
					lcd1602_Print_text(lcd1602_tx_buffer);
				} else if (procent >= 75) {
					sprintf(lcd1602_tx_buffer, "%ca%cpy%c%ca...                       ", RU_Z, RU_g, RU_z, RU_k);
					lcd1602_Print_text(lcd1602_tx_buffer);
				}

				procent = procent + 4;
				sprintf(lcd1602_tx_buffer, "%d%c", procent, 0x25);
				lcd1602_SetCursor(10, 1);
				lcd1602_Print_text(lcd1602_tx_buffer);
				i++;
				if (i == 25) {
					Delay = 300;
					flag = 1;
					i = 0;
					lcd1602_Clean();
					lcd1602_Backlight(0);
				}
			}
			if (flag == 1) {
				lcd1602_SetCursor(0, 0);
				sprintf(lcd1602_tx_buffer, "Tec%c %c%cc%c%ce%c", RU_t, RU_d, RU_i, RU_p, RU_l, RU_ya);
				lcd1602_Print_text(lcd1602_tx_buffer);
				i++;
				if (i > 5) {
					lcd1602_Backlight(1);
					if (i == 10) {
						flag = 2;
						i = 0;
						lcd1602_Clean();
					}
				}
			}

			if (flag == 2) {
				lcd1602_SetCursor(0, 0);
				sprintf(lcd1602_tx_buffer, "                Soldering iron");
				lcd1602_Print_text(lcd1602_tx_buffer);

				lcd1602_SetCursor(0, 1);
				sprintf(lcd1602_tx_buffer, "                %cp%c%ce%c, %ca%c %ce%ca?)", RU_P, RU_i, RU_v, RU_t, RU_k, RU_k, RU_d, RU_l);
				lcd1602_Print_text(lcd1602_tx_buffer);
				lcd1602_Move_to_the_left();
				i++;
				if (i == 35) {
					flag = 3;
					i = 0;
					lcd1602_Clean();
				}
			}
			if (flag == 3) {
				i++;
				if (i < 5) {
					lcd1602_SetCursor(0, 0);
					sprintf(lcd1602_tx_buffer, "Hy %cpo%ce %cop%c :)", RU_v, RU_d, RU_n, RU_m);
					lcd1602_Print_text(lcd1602_tx_buffer);
				}
				if (i >= 5 && i < 10) {
					lcd1602_Clean();
					lcd1602_SetCursor(0, 1);
					lcd1602_Print_text(lcd1602_tx_buffer);

				} else if (i >= 10 && i < 15) {
					flag = 4;
					i = 0;
					lcd1602_Clean();
					Delay = 200;
				}
			}
			if (flag == 4) {
				if (i < 14) {
					lcd1602_SetCursor(0, 0);
					lcd1602_Print_text(":)");
					lcd1602_Move_to_the_right();
					i++;
				} else if (i == 14) {
					lcd1602_Clean();
					lcd1602_SetCursor(14, 1);
					lcd1602_Print_text(":)");
					i++;
				} else if (i > 14) {
					lcd1602_Move_to_the_left();
					i++;
					if (i == 30) {
						flag = 5;
						i = 0;
						lcd1602_Clean();
						Delay = 100;
					}
				}

			}
			if (flag == 5) {
				lcd1602_SetCursor(0, 0);
				sprintf(lcd1602_tx_buffer, "int = %d            ", counter);
				lcd1602_Print_text(lcd1602_tx_buffer);

				lcd1602_SetCursor(0, 1);
				sprintf(lcd1602_tx_buffer, "float = %.3f        ", counter_float);
				lcd1602_Print_text(lcd1602_tx_buffer);

			}

		}

		if (HAL_GetTick() - T2 >= 10) {
			T2 = HAL_GetTick();
			counter++;
			counter_float = counter_float + 0.264f;

		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 255;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

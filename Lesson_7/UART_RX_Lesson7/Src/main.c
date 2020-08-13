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
#define size_tx_buffer 10 //Длина буфера tx
bool flag_send; //флаг на разрешение вывода tx_buffer[] в serial port
uint8_t queue_message; //очередь сообщений (т.к. работает с прерываниями или DMA. см. урок 6)
uint8_t tx_buffer[size_tx_buffer]; //собственно tx_buffer, который будет выводить в порт то, что принял МК по rx.
uint8_t rx_buffer[1]; // Буфер, который будет принимать информацию. (По одному символу, чтоб постоянно в прерывание заходить и обрабатывать входящее сообщение.)
uint8_t error_message[] = "tx buffer is crowded\n\r"; //Сообщение о том, что tx_buffer[size_tx_buffer] переполнен.
uint8_t error_counter; // служит для того, чтоб сообщение "tx buffer is crowded\n\r" выводилось только раз.
volatile uint8_t rx_counter; // Счетчик для входящих символов.
volatile uint8_t size_message; // Размер сообщения для вывода в порт.
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
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {//Символ принялся, срабатывает прерывание, здесь мы будем уже что-то делать с принятыми данными.
	if (rx_buffer[0] != '\r') { //если входящий символ не равен '\r'
		tx_buffer[rx_counter] = rx_buffer[0]; //добавим этот символ в tx_buffer
		rx_counter++;//увеличиваем счетчик входящих символов
		size_message = rx_counter; //размер сообщения, которое в сериал порт будет отправляться
		if (size_message >= size_tx_buffer && error_counter == 0) {//если размер сообщения превышает размер буфера, то выводим сообщение, что буфер переполнен.
			HAL_UART_Transmit_DMA(&huart1, error_message,
					sizeof error_message / sizeof error_message[0]);
			error_counter++;//счетчик вывода сообщений ошибки. Чтоб отправилось только раз.
			flag_send = 0;//запрещаем выводить сообщение в порт, ведь оно превышено по размеру.
		}
	} else if (rx_buffer[0] == '\r') {//если же входящий символ равен '\r'
		tx_buffer[rx_counter] = '\n';//то наше сообщение полностью получено. Давайте добавим к нему перевод на следующую строку
		tx_buffer[rx_counter + 1] = '\r';//так же добавим возврат каретки
		tx_buffer[rx_counter + 2] = '\0';//ручками пропишем нулевой символ, т.к. он сам себя не добавит
		size_message = rx_counter + 3;//соответственно мы должны увеличить размер отправляемого сообщения в порт.
		rx_counter = 0;//сбросим счетчик входящий символов, чтоб заново получать сообщение.
		if (size_message >= size_tx_buffer) {//опять же, если размер сообщения превышает размер буфера, то выводим сообщение, что буфер переполнен.
			flag_send = 0;////запрещаем выводить сообщение в порт, ведь оно превышено по размеру.
			HAL_UART_Transmit_DMA(&huart1, error_message,
					sizeof error_message / sizeof error_message[0]);
		} else {
			flag_send = 1;//если же сообщение не превышено по размеру tx_buffer, то разрешаем выводить в порт то, что вывели
			error_counter = 0;//сбросим счетчик ошибок
		}
	}
	HAL_UART_Receive_IT(&huart1, rx_buffer, 1);//Запускаем прием данных после каждого прерывания.
}
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
	MX_DMA_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	flag_send = 0;//Отправка сообщений еще запрещена, ведь отправлять еще нечего.
	queue_message = 1;//очередь отправки сообщений изначально равна 1
	rx_counter = 0;//Символы еще не приходили. Будет 0.
	size_message = 0;//Размера отправляемого сообщения еще не знаем. Будет 0.
	error_counter = 0;//Ошибок еще не было. Будет 0.
	HAL_UART_Receive_IT(&huart1, rx_buffer, 1);//Разрешаем прием данных, размером в 1 символ.
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		if (flag_send == 1 && queue_message != 255) {//если разрешено отсылать сообщение и очередь не равна максимальной, то
				if (queue_message == 1 && huart1.gState == HAL_UART_STATE_READY) { //если порт свободен, и очередь сообщения соответствует, то
				HAL_UART_Transmit_DMA(&huart1, tx_buffer, size_message);//отправляем сообщение, чего нам там пришло в rx_buffer
				queue_message = 255;//ставим следующую очередь, если захотим еще что-то отсылать в порт.
			}
		} else if (flag_send == 1 && queue_message == 255) {//если же очередь дошла до максимальной, то
			queue_message = 1;//сбросим очередь отправки, чтоб сообщения снова могли по очереди отсылаться
			flag_send = 0;//отправку сообщений запретим. Разрешение получим в прерывании.
		}
		if (tx_buffer[0] == 'O' && tx_buffer[1] == 'n' && tx_buffer[2] == '\n' && tx_buffer[3] == '\r') { //Если в tx_buffer пришло 'On\r', то
			GPIOC->BRR = LED_Pin; //включим 13 светодиод
		} else if (tx_buffer[0] == 'O' && tx_buffer[1] == 'f' && tx_buffer[2] == 'f' && tx_buffer[3] == '\n' && tx_buffer[4] == '\r') { //Если в tx_buffer пришло 'Off\r', то
			GPIOC->BSRR = LED_Pin; //выключим 13 светодиод
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

	/** Initializes the CPU, AHB and APB busses clocks
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
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/** 
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	/* DMA1_Channel5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : LED_Pin */
	GPIO_InitStruct.Pin = LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

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
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

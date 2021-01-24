/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.cpp
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 *  Created on: 25 дек. 2020 г.
 *      Author: Oleg Volkov
 *
 *  YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
 *  GitHub: https://github.com/Solderingironspb/Lessons-Stm32/blob/master/README.md
 *  Группа ВК: https://vk.com/solderingiron.stm32
 * 
 ******************************************************************************
 */
/* USER CODE END Header */
/* USER CODE BEGIN Includes */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "uart_mqtt_lib.h"

#define VARIABLE_ID esp_rx_buffer[1]
#define name_mqtt_user "Stm32_F103C8T6"
/*-------------------------------Настройки Wi-fi------------------------------------------*/

const char *ssid = "Dom";       // Имя Wi-fi точки доступа
const char *pass = "4815162342";  // Пароль от Wi-fi точки доступа

const char *mqtt_server = "tailor.cloudmqtt.com";  // Имя сервера MQTT
const int mqtt_port = 15835;                       // Порт для подключения к серверу MQTT
const char *mqtt_user = "dcdjpkvp";                // Логин от сервера
const char *mqtt_pass = "d0wyfc9x25Ce";            // Пароль от сервера

/*-------------------------------Настройки Wi-fi------------------------------------------*/

uint8_t esp_rx_buffer[9] = {
    0,
};  //входящий буфер
uint8_t esp_tx_buffer[9] = {
    0,
};  //исходящий буфер

bool LED = 0;          //Светодиод.   Адрес: 0x02
float counter = 0.0f;  //Счетчик.     Адрес: 0x03

/* USER CODE END Includes */

WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);

/* USER CODE BEGIN PFP */
void callback(const MQTT::Publish &pub);
void uart_parsing(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void callback(const MQTT::Publish &pub)  // Функция получения данных от сервера
{
  String payload = pub.payload_string();

  /*------------------Парсинг приходящих писем в топики-----------------------*/

  if (String(pub.topic()) == "test/LED")  //  проверяем из нужного ли нам топика пришли данные
  {
    int test_LED = payload.toInt();  //  преобразуем полученные данные в тип integer(все типы данных, кроме float преобразуем в integer)

    if (test_LED == 1) {
      UART_MQTT_Send_data_bool(0x02, test_LED, esp_tx_buffer);
    } else if (test_LED == 0) {
      UART_MQTT_Send_data_bool(0x02, test_LED, esp_tx_buffer);
    }
  }

  if (String(pub.topic()) == "test/Counter_float")  //  проверяем из нужного ли нам топика пришли данные
  {
    counter = payload.toFloat();  //  преобразуем полученные данные в тип float(все типы данных, кроме float преобразуем в integer)
    UART_MQTT_Send_data_float(0x03, counter, esp_tx_buffer);
  }

  /*------------------Парсинг приходящих писем в топики-----------------------*/
}

void uart_parsing(void) {
  if (Serial.available() > 0) {
    Serial.readBytes(esp_rx_buffer, 9);
    switch (VARIABLE_ID) {
        /*----------------здесь будет выборка по номеру переменной-----------------------*/

      case (0x02):
        if (UART_MQTT_Checksumm_validation(esp_rx_buffer)) {
          LED = UART_MQTT_Receive_data_bool(esp_rx_buffer);
          client.publish("test/LED", String(LED));
        }
        break;
      case (0x03):
        if (UART_MQTT_Checksumm_validation(esp_rx_buffer)) {
          counter = UART_MQTT_Receive_data_float(esp_rx_buffer);
          client.publish("test/Counter_float", String(counter));
        }
        break;
        

        /*----------------здесь будет выборка по номеру переменной-----------------------*/
    }
    memset(esp_rx_buffer, 0, 9);  //очистка входящего буффера
  }
}

/* USER CODE END 0 */

void setup(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Initialize all configured peripherals */
  Serial.begin(115200);
  delay(100);
  Serial.setTimeout(5);

  /* USER CODE BEGIN 2 */
  LED = 0;
}
/* USER CODE END 2 */

/* Infinite loop */
/* USER CODE BEGIN WHILE */
void loop(void)
/* USER CODE END WHILE */
/* USER CODE BEGIN 3 */
{
  if (WiFi.status() != WL_CONNECTED)  //Проверяем статус подключения к Wi-fi точке
  {
    wifi_not_ok();  //отсылаем служебную команду Wifi != OK
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    wifi_ok();  //отсылаем служебную команду Wifi = OK
  }

  if (WiFi.status() == WL_CONNECTED)  // Если подключились к Wi-fi точке
  {
    if (!client.connected())  //Проверяем статус подключения к MQTT серверу
    {
      mqtt_not_ok();                                                                     //отсылаем служебную команду MQTT = !OK
      if (client.connect(MQTT::Connect(name_mqtt_user).set_auth(mqtt_user, mqtt_pass)))  //Если подключились к MQTT серверу, то авторизуемся
      {
        mqtt_ok();  //отсылаем служебную команду MQTT = OK
        client.set_callback(callback);

        /*--------------------Указываем топики, на которые хотим подписаться-------------------------*/
        client.subscribe("test/Counter_float");
        client.subscribe("test/LED");
        /*--------------------Указываем топики, на которые хотим подписаться-------------------------*/
      } else {
        mqtt_not_ok();  //отсылаем служебную команду MQTT = !OK
      }
    }

    if (client.connected()) {
      client.loop();
      uart_parsing();
    }
  }
}
/* USER CODE END 3 */
/************************** (C) COPYRIGHT Soldering iron *******END OF FILE****/

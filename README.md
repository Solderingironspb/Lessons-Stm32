# STM32 Virtual COM Port.
![VirtualComPort](https://user-images.githubusercontent.com/68805120/115999690-61b8be80-a5f5-11eb-8e10-216d91460310.jpg)
Видео: [STM32 Virtual COM Port. О ценах на МК. Что делать дальше?](https://youtu.be/ipsWfXxMgoA)
###
**Небольшое дополнение для тех, у кого устройство определится, но к терминалу подключаться не сможет. У меня данная проблема проявилась на Stm32F401CEU6, при смене кварца с 25 на 8 MHz. Пробовал на нескольких компьютерах. Переустанавливал драйвера и прочее. Уж не знаю, как это все связано....но я в интернете не один такой:**
###
**Пример проблемы:**
![wrqFq5Tdpoo](https://user-images.githubusercontent.com/68805120/117110803-5bfa7000-ad8f-11eb-9b35-4d9f1b084efe.jpg)
**Говорит, что порт занят, но мы его ничем не занимали. Либо же, при использовании Arduino IDE, к монитору порта подключается, но пишет, что ошибка при настройке параметров последовательного порта.**
### 
**Решение проблемы:**
###
**Попробуйте добавить следующее в файл usbd_cdc_if.c:**
##
**После шапки из комментариев к конфигурации VCP**
##
```/*******************************************************************************/
/* Line Coding Structure                                                       */
/*-----------------------------------------------------------------------------*/
/* Offset | Field       | Size | Value  | Description                          */
/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
/* 4      | bCharFormat |   1  | Number | Stop bits                            */
/*                                        0 - 1 Stop bit                       */
/*                                        1 - 1.5 Stop bits                    */
/*                                        2 - 2 Stop bits                      */
/* 5      | bParityType |  1   | Number | Parity                               */
/*                                        0 - None                             */
/*                                        1 - Odd                              */
/*                                        2 - Even                             */
/*                                        3 - Mark                             */
/*                                        4 - Space                            */
/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
/*******************************************************************************/
static uint8_t lineCoding[7] // 115200bps, 1stop, no parity, 8bit
    = { 0x00, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x08 }; 
```
##
**Также следует после этого поправить функцию CDC_Control_FS:**

##
```
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
   ...
    case CDC_SET_LINE_CODING:
      memcpy(lineCoding, pbuf, sizeof(lineCoding));
    break;

    case CDC_GET_LINE_CODING:
      memcpy(pbuf, lineCoding, sizeof(lineCoding));
    break;
    ...
}
```
##
**Скриншот для большего понимания:**
![wvlWxB6yB58](https://user-images.githubusercontent.com/68805120/117111837-dc6da080-ad90-11eb-8fc4-1017aaab0d37.jpg)

##
**После этого у меня все завелось.**





Материалы из видео **[[Скачать]](https://github.com/Solderingironspb/Lessons-Stm32/archive/Virtual_Com_Port.zip)**
**[[Главная страница]](https://github.com/Solderingironspb/Lessons-Stm32/blob/master/README.md)**

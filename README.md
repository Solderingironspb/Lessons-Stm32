# Перестаем использовать -u_printf_float
![Float_transform](https://user-images.githubusercontent.com/68805120/130028483-3e806f1b-9d90-4a32-bdd2-204a0e92f0ed.jpg)
###
**Простенькая библиотека для разбития float на две части: целую и дробную.** 
###
**Это нужно, чтоб не использовать -u_printf_float в линкере, дабы сэкономить память МК.**
###
**В библиотеке используется округление.** 
###
*Например: Val = 25.436f. Мы хотим вывести число с 2 знаками после запятой. Получим 25.44*
##
**Пример работы с библиотекой:**
##
```
//Файл main.c
 #include "Float_transform.h"
 extern int integer_number;
 extern uint32_t fractional_number;

 float Val = -3.14159265359;

 Float_transform(Val, 4, &integer_number, &fractional_number);
 printf("float = %d.%.04d\r\n", integer_number, fractional_number);*/
//Получим вывод: float = -3.1416
/*
 * Обратите внимание на вывод чисел. Сколько знаков после запятой, столько и отступ дробного числа при печати.
 * Т.к. если Val = 25.0126;
 * и мы сделаем
 * Float_transform(Val, 3, &integer_number, &fractional_number);
 * то
 * integer_number = 25;
 * fractional_number = 13;
 * а вывод должен быть 25.013, поэтому:
 * printf("float = %d.%.03d\r\n", integer_number, fractional_number);
 *
 * Получим вывод: float = 25.013
 *
 */ 
```
##

Материалы из видео **[[Скачать]](https://github.com/Solderingironspb/Lessons-Stm32/archive/Float_transform.zip)**
**[[Главная страница]](https://github.com/Solderingironspb/Lessons-Stm32/blob/master/README.md)**

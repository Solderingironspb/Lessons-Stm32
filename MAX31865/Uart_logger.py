# Прием данных из Serial port.
# by Oleg Volkov 2021
# Документация по библиотеке pySerial https://pyserial.readthedocs.io/en/latest/pyserial_api.html
# P.S. написано на скорую руку. В Python я особо не силен. Работает и ладно.

# ======================ПОДКЛЮЧЕНИЕ БИБЛИОТЕК======================== #
import serial  # Библиотека для работы с COM портом
import csv  # Работа с *.CSV файлами
import serial.tools.list_ports
from datetime import datetime
import time
import struct
import smtplib as smtp  #не используется
from email.mime.text import MIMEText  #не используется
from email.header import Header  #не используется

# ======================ПОДКЛЮЧЕНИЕ БИБЛИОТЕК======================== #

# ======================ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ======================== #
rx_buffer = []  # Входящий буфер данных
cnt = 0  # Счетчик входящих сообщений
flag_rec_csv = 0  # Флаг разрешения записи *.CSV файла
recording_buffer = []  # Буфер, который будем кидать в *.CSV файл
Temperature_float = 0  # Температура °C. Считываем с датчика PT100


# ======================ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ======================== #


# ====================ПОЛЬЗОВАТЕЛЬСКИЕ ФУНКЦИИ====================== #

# --------------Просмотреть список доступных COM портов------------- #
def WINDOWS_COM_PORT_LIST():
    print("Список доступных COM портов:\r")
    ports = list(serial.tools.list_ports.comports(0))
    for p in ports:
        print(p, "\r")
# --------------Просмотреть список доступных COM портов------------- #


# --------------Функция чтения данных из COM порта------------------- #
def UART_Receive():
    global rx_buffer
    while (UART.in_waiting) > 0:
        rx_buffer = UART.read(UART.in_waiting)
        global cnt
        global recording_buffer
        if len(rx_buffer) > 8:  # если в буфер данных что-то приходит, то
            print("\rПринято байт:", len(rx_buffer), end="; Данные:")
            for i in range(len(rx_buffer)):
                recording_buffer.append(hex(rx_buffer[i]))
                print(hex(rx_buffer[i]), end=" ")
            print("; Пакетов принято:", cnt + 1, end="")

            # -----------------------------Проверка контрольной суммы------------------------------------------------ #
            CRC_rx_buffer = (~(rx_buffer[1] + rx_buffer[2] + rx_buffer[3] + rx_buffer[4] + rx_buffer[5] + rx_buffer[6])) + 1
            # print(hex(CRC_rx_buffer & 2 ** 16 - 1))
            CRC_rx_buffer_string = format(CRC_rx_buffer & (2 ** 16 - 1), 'x')
            CRC_rx_buffer_check = (rx_buffer[7] << 8) | (rx_buffer[8])
            # print(hex(CRC_rx_buffer_check & 2 ** 16 - 1))
            CRC_rx_buffer_check_string = format(CRC_rx_buffer_check & (2 ** 16 - 1), 'x')
            if CRC_rx_buffer_string != CRC_rx_buffer_check_string:
                print("\rCRC !=OK")
            else:
                print("\rCRC = OK")
                # -----------------------------Получение данных о температуре из буфера---------------------------------- #
                Temperature = (rx_buffer[3] << 24) | (rx_buffer[4] << 16) | (rx_buffer[5] << 8) | (rx_buffer[6])
                Temperature_string = format(Temperature & (2 ** 32 - 1), 'x')
                # print(Temperature_string)
                global Temperature_float
                Temperature_float = (struct.unpack("!f", bytes.fromhex(Temperature_string))[0])
                Temperature_float = str(Temperature_float).replace('.', ',')  # Для Excel, заменим . на ,
                print("\rТемпература = ", Temperature_float)
                # -----------------------------Получение данных о температуре из буфера---------------------------------- #

            # -----------------------------Проверка контрольной суммы------------------------------------------------ #

            cnt = cnt + 1
            print("Принято пакетов = \r", cnt)
            global flag_rec_csv
            flag_rec_csv = 1


# --------------Функция чтения данных из COM порта------------------- #


# ====================ПОЛЬЗОВАТЕЛЬСКИЕ ФУНКЦИИ====================== #


# ============================ИНИЦИАЛИЗАЦИЯ========================== #
WINDOWS_COM_PORT_LIST()
COM = str(input("Какой COM_port использовать? Напишите, например '"'COM1'"' \r\n"))
UART = serial.Serial(COM, baudrate=115200, timeout=0.000694)  # 0.0000000001   0.000694
if UART.is_open:
    print("Вы подключились к: " + UART.portstr + '\n')
    with open(r"C:\python\test_logger.csv", 'a', newline='', encoding='utf-8') as csvfile:  # Параметр 'a' - прочесть файл и добавить данные в конец. 'w' создать новый чистый файл
        writer = csv.writer(csvfile, delimiter=";")
        writer.writerow(["Time", "Temperature Celsius"])

        # ============================ИНИЦИАЛИЗАЦИЯ========================== #

        # ===========================ОСНОВНОЙ ЦИКЛ========================== #
        while 1:
            UART_Receive()
            if flag_rec_csv == 1:
                Time_str = (
                    f"""{'{:02}'.format(datetime.today().day)}.{':02'.format(datetime.today().month)}.{':02'.format(datetime.today().year)} {'{:02}'.format(datetime.today().hour)}:{'{:02}'.format(datetime.today().minute)}:{'{:02}'.format(datetime.today().second)}""")
                writer.writerow([f"""{datetime.today().date()} {'{:02}'.format(datetime.today().hour)}:{'{:02}'.format(datetime.today().minute)}:{'{:02}'.format(datetime.today().second)} MSK""",
                                 Temperature_float])

                csvfile.flush()
                time.sleep(0.100)
                recording_buffer = []
                flag_rec_csv = 0

        # ===========================ОСНОВНОЙ ЦИКЛ========================== #

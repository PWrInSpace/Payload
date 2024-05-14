#!/usr/bin/python3

import serial
import sys
import time

# Ustawienia portu szeregowego
ser = serial.Serial(
    port=sys.argv[1],
    timeout=1  # Timeout odczytu w sekundach
)

# Wysłanie litery "G" na port szeregowy
ser.write(b'G')

# Odbieranie danych z portu szeregowego
time.sleep(0.01)
while True:

    data = ser.read()  # Odczyt danych (maksymalnie 1024 bajtów)
    if data:
        # Jeśli dane zostały odebrane, zapisz je do pliku
        with open("received_data.bin", "ab") as file:
            file.write(data)
    else:
        break

ser.close()

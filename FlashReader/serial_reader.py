import serial
import sys
import time

def read_from_serial(serial_port: str, output_bin_name: str):

    # Ustawienia portu szeregowego
    ser = serial.Serial(
        port=serial_port,
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
            with open(output_bin_name, "ab") as file:
                file.write(data)
        else:
            break

    ser.close()

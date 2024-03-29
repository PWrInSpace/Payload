#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np
import struct

class Frame:

    time_us = 0
    x = 0
    y = 0
    z = 0
    state = 0

    def toString(self):
        return f"{self.time_us}, {self.x}, {self.y}, {self.z}, {self.state}"

frames = []

def divide_binary_file(input_file_path):
    try:
        # Otwórz plik wejściowy do odczytu binarnego
        with open(input_file_path, 'rb') as file:
            # Odczytaj całą zawartość pliku
            file_data = file.read()

            # Oblicz ilość strumieni
            stream_size = 2048
            num_streams = len(file_data) // stream_size

            # Dziel plik na strumienie binarne
            streams = [file_data[i * stream_size:(i + 1) * stream_size] for i in range(num_streams)]

            print(f"Plik został podzielony na {num_streams} strumieni binarnych.")
            return streams
    except FileNotFoundError:
        print("Podany plik nie istnieje.")
    except Exception as e:
        print(f"Wystąpił błąd: {e}")

# Przykładowe użycie
input_file_path = 'LOT.BIN'
binary_streams = divide_binary_file(input_file_path)

j = 0
k = 0
for bin_stream in binary_streams:

    format_string = 'I' + 'B' * 2040 + 'BBBB'

    # Rozpakowanie danych binarnych
    unpacked_data = struct.unpack(format_string, bin_stream)

    for i in range(0, 680):

        frame = Frame()

        frame.time_us = j
        frame.x = unpacked_data[i * 3 + 1]
        frame.y = unpacked_data[i * 3 + 2]
        frame.z = unpacked_data[i * 3 + 3]
        frame.state = unpacked_data[-4]

        frames.append(frame)

        j+= 0.0001

t = []
x = []

for frame in frames:
    t.append(frame.time_us)
    x.append(frame.y)

# Usunięcie składowej stałej:
x = x - np.mean(x)

# sampling rate
sr = 10000
# sampling interval
ts = 1.0/sr
#t = np.arange(0,len(x)/sr,ts)

X = np.fft.fft(x)
N = len(X)
n = np.arange(N)
T = N/sr
freq = n/T

# Wyświetlanie sygnału i jego FFT
plt.figure(figsize=(12, 6))

# Wykres sygnału
plt.subplot(2, 1, 1)
plt.plot(t, x)
plt.title('x(t)')
plt.xlabel('Czas [s]')
plt.ylabel('Amplituda')

# Wykres FFT
plt.subplot(2, 1, 2)
plt.plot(freq, np.abs(X))
plt.title('x(f)')
plt.xlabel('Częstotliwość [Hz]')
plt.ylabel('Amplituda FFT')
plt.xlim(-100, 5000)

plt.tight_layout()
plt.show()

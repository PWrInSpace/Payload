#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np
import math
import csv
import sys

class Frame:

    time_us = 0
    x = 0
    y = 0
    z = 0
    state = 0

    def toString(self):
        return f"{self.time_us}, {self.x}, {self.y}, {self.z}, {self.state}"

frames = []

# Read the csv given as argument:
with open(sys.argv[1], newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=";")
    for row in spamreader:

        frame = Frame()
        frame.time_us = float(row[0])
        frame.x =       int(row[1])
        frame.y =       int(row[2])
        frame.z =       int(row[3])
        frame.state =   int(row[4])
        frames.append(frame)

t = []
x = []
y = []
z = []

for frame in frames:
    t.append(frame.time_us)
    x.append(frame.x)
    y.append(frame.y)
    z.append(frame.z)

# Remove constant part:
x = x - np.mean(x)
y = y - np.mean(y)
z = z - np.mean(z)

# Change raw adc value to acceleration:
# For VSA004 0.304 and for ADXL 0.647.
factor = 0.647
for i in range(len(x)):
    x[i] *= factor
    y[i] *= factor
    z[i] *= factor

# sampling rate 10kHz:
sr = 4000
# sampling interval:
ts = 1.0/sr

X = np.fft.fft(x)
Y = np.fft.fft(y)
Z = np.fft.fft(z)
N = len(X)
n = np.arange(N)
T = N/sr
freq = n/T

# Normalize FFT values:
for i in range(N):
    X[i] /= N
    Y[i] /= N
    Z[i] /= N

# Generate plots:
plt.figure(figsize=(12, 6))

# Time signal:
plt.subplot(2, 1, 1)
plt.plot(t, x)
#plt.plot(t, y)
#plt.plot(t, z)
plt.title('x(t)')
plt.xlabel('Czas [s]')
plt.ylabel('Amplituda [g]')

# FFT plot:
plt.subplot(2, 1, 2)
plt.plot(freq, np.abs(X))
plt.plot(freq, np.abs(Y))
plt.plot(freq, np.abs(Z))
plt.title('x(f)')
plt.xlabel('Częstotliwość [Hz]')
plt.ylabel('PDS [g^2/Hz]')
plt.xlim(-50, 2000)

plt.tight_layout()
plt.show()

g_rms_x = math.sqrt(np.trapz(np.abs(X), t)/2)
g_rms_y = math.sqrt(np.trapz(np.abs(Y), t)/2)
g_rms_z = math.sqrt(np.trapz(np.abs(Z), t)/2)
print(f"G_RMS osi x wynosi: {g_rms_x}g")
print(f"G_RMS osi y wynosi: {g_rms_y}g")
print(f"G_RMS osi z wynosi: {g_rms_z}g")

plt.figure(figsize=(12, 4))

plt.subplot(1, 3, 1)
plt.specgram(x, Fs=1/(t[1]-t[0]), cmap='viridis')

plt.subplot(1, 3, 2)
plt.specgram(y, Fs=1/(t[1]-t[0]), cmap='viridis')

plt.subplot(1, 3, 3)
plt.specgram(z, Fs=1/(t[1]-t[0]), cmap='viridis')
plt.show()

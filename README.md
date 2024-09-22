# Payload

This is a payload repository. It contains 3 subprojects:
 - Payload Control firmware,
 - Payload Measure firmware,
 - DataVisualizer data manipulation software.

## Payload Control firmware

Payload Control firmware is an ESP32 project written in ESP-IDF based on `ESP-Now-Slave` template. It uses deep sleep, ESP NOW and state machine to communicate with OBC and turn on the power of Measure boards during the right state. Payload control gives 3 signals to Measure board:
 - Power enable (for states from launch to second separation),
 - Write mode (high state - write data, low state - read data),
 - State number (UART Tx).

## Payload Measure firmware

Payload Measure firmware is a STM32 project written in Cube IDE. It uses ADC and save data in NAND flash memory. It uses internal flash to store last used slot to not overwrite the last test / flight. Payload measure gets 3 signals from Control board:
 - Power enable (for states from launch to second separation),
 - Write mode (high state - write data, low state - read data),
 - State number (UART Rx).

## DataVisualizer

DataVisualizer is a tool designed for visualization of vibration data downloaded from ESP32S3 measurement board with the WiFi.

Usage:
1. `main.py` - convert binary data into csv. Eg:
```
./main.py data.bin
```
2. `plot_results.py` - plot the csv data with Fourier and G_RMS. Eg:
```
plot_results.py payload_data_2024-09-22_154213.csv
```

Requires:
 - numpy,
 - matplotlib.

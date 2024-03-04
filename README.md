# Payload

This is a payload repository. It contains 3 subprojects:
 - Payload Control firmware,
 - Payload Measure firmware,
 - DraPaViz data manipulation software.

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

## DraPaViz

DraPaViz is `Drgania`, `Payload` and `Vizulatization` software written in python.

TODO
#ifndef PINOUT_H
#define PINOUT_H
#include <Arduino.h>


#define BATT_CHECK 36

// SPI
#define MOSI 13
#define MISO 12
#define SCK 14
#define SD_CS 16

// I2C
#define SCL 22
#define SDA 21

// RASPBERRY AND LED CONTROL
#define RPI_PIN_16 5
#define RPI_PIN_18 18
#define RPI_PIN_22 19
#define RPI_POWER_PIN 23
#define RPI_LED 2

// LED pins
#define LED_RPI_READY 33
#define LED_RPI_RECORDING 32
#define LED_SD_GOOD 25

void pinInit();

#endif
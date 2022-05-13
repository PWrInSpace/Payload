#ifndef PINOUT_H
#define PINOUT_H
#include <Arduino.h>
#include <FreeRTOS.h>

#define BATT_CHECK 36 // Sens VP

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
#define WAKE_LED 2

void pinInit();

#endif
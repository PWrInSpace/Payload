#ifndef PINOUT_H
#define PINOUT_H

// I2C listener number
#define I2C_NUMBER 2
#define WDT_TIMEOUT 3

// ADC
#define ADC_SAMPLES 10

// SPI:
#define MOSI_PIN 23
#define MISO_PIN 19
#define SCLK_PIN 18
#define SD_CS_PIN 5

// I2C:
#define I2C_SCL 22
#define I2C_SDA 21

// Communication
#define DATA_COLLECTION_READY 13
#define DATA_SAVE_PERMISSION 14

// LED Switch
#define LED_SWITCH 4

// ADC measurements:
#define ADC_GPIO34 34
#define ADC_GPIO35 35
#define ADC_GPIO32 32
#define ADC_GPIO33 33
#define ADC_GPIO25 25
#define ADC_GPIO26 26
#define ADC_GPIO27 27

// Boot button:
#define BOOT_BUTTON_PIN 0

#endif
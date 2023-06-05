#ifndef PINOUT_H
#define PINOUT_H

// IMPORTANT! Check the pinout.

// SPI:
#define MISO_PIN 27
#define SCLK_PIN 12
#define MOSI_PIN 15
#define SD_CS_PIN 2

// I2C:
#define I2C_SCL 22
#define I2C_SDA 21

// INFO LED
#define LED_GREEN 17
#define LED_YELLOW 16

// Detection power
#define DETECTION_MOSFET_PIN 23

// Battery divider
#define INPUT_VOLTAGE_PIN 36

// Boot button:
#define BOOT_BUTTON_PIN 0

#define MOSFET_ON 0
#define MOSFET_OFF 1

#define VBAT_PIN 36

#endif
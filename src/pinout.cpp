#include "../include/pinout.h"

void pinInit()
{
    pinMode(RPI_PIN_16, OUTPUT);
    pinMode(RPI_PIN_18, INPUT);
    pinMode(RPI_PIN_22, INPUT);
    pinMode(RPI_POWER_PIN, OUTPUT);
    pinMode(WAKE_LED, OUTPUT);
    // pinMode(BATT_CHECK, INPUT);
    pinMode(SD_CS, OUTPUT);
    // pinMode(LED_RPI_READY, OUTPUT);
    // pinMode(LED_RPI_RECORDING, OUTPUT); //cholera te piny są input only
    // pinMode(LED_SD_GOOD, OUTPUT);
}
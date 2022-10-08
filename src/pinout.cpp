#include "../include/pinout.h"

void pinInit()
{
    pinMode(RPI_PIN_16, OUTPUT);
    pinMode(RPI_PIN_18, INPUT);
    pinMode(RPI_PIN_22, INPUT);
    pinMode(RPI_POWER_PIN, OUTPUT);
    pinMode(RPI_LED, OUTPUT);
    pinMode(BATT_CHECK, INPUT);
    pinMode(SD_CS, OUTPUT);
    pinMode(LED_RPI_READY, OUTPUT);
    pinMode(LED_RPI_RECORDING, OUTPUT);
    pinMode(LED_SD_GOOD, OUTPUT);

    digitalWrite(RPI_LED,LOW);

    /// @brief
    //initalize all LED states at the beginning
    digitalWrite(LED_RPI_READY, LOW);
    digitalWrite(LED_RPI_RECORDING, LOW);
    digitalWrite(LED_SD_GOOD, LOW);

}
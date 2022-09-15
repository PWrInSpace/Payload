#include "../include/pinout.h"

void pinInit()
{
    pinMode(BATT_CHECK, INPUT);
    pinMode(SD_CS, OUTPUT);
}
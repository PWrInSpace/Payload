#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "FS.h"
#include "SPIFFS.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "ESPAsyncWebServer.h"
#include <ESPmDNS.h>

#include "pinout.h"
#include "dataStructs.h"

bool isWritingMode();
void writeData();
void readData();
void uartCommTask();
void flashTask();
void blinkNTimes(uint8_t n);

#endif

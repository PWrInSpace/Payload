#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#define ANSWER_WAIT_MS 750
#define MIN_SLEEP_TIME_MS 2000
#define SLEEP_TIME_MS 30000

#define IDNAME(name) #name

#include "pinout.h"
#include "stdint.h"
#include "Arduino.h"

// Payload internal state
enum PayloadInternalState {
    EMPTY = 0,
    AWAIT,
    READY,
    WRITE,
};

static const char* payloadInternalStateNames[] = {
    IDNAME(EMPTY),
    IDNAME(AWAIT),
    IDNAME(READY),
    IDNAME(WRITE)
};

struct AdcReadOut {
    uint8_t PWMLevel;
    uint16_t gpio_34;
    uint16_t gpio_35;
    uint16_t gpio_32;
    uint16_t gpio_33;
    uint16_t gpio_25;
    uint16_t gpio_26;
    uint16_t gpio_27;
};

struct DataToSave {

    AdcReadOut adcMeasurement;

    String toString() {

        char outStr[50];
        sprintf(outStr, "%d:%d,%d,%d,%d,%d,%d,%d\n",
         adcMeasurement.PWMLevel,
         adcMeasurement.gpio_34,
         adcMeasurement.gpio_35,
         adcMeasurement.gpio_32,
         adcMeasurement.gpio_33,
         adcMeasurement.gpio_25,
         adcMeasurement.gpio_26,
         adcMeasurement.gpio_27
        );

        return String(outStr);
    }
};

struct ModuleData {

    DataToSave dataToSave;

    uint8_t payloadState = AWAIT;

    const bool flashSave = true;
    const bool sdSave = true;
};

extern volatile ModuleData moduleData;

#endif
#include "../../include/com/functions.h"

// To implementation of each module:

void initPeripherals() {


    
}

/**********************************************************************************************/

// To implementation of each module:

void measure() {


}

/**********************************************************************************************/

// To implementation of each module:

void rxNowHandler(const uint8_t *incomingData, int len) {


}

/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/

uint32_t getPowerTime_ms() {

    timeval tv;
    timezone tz;

    gettimeofday(&tv, (void*) &tz);
    return tv.tv_sec * 1000 + tv.tv_usec/1000;
}

/**********************************************************************************************/

void goToSleep() {

    dataToObc.wakenUp = false;
    esp_sleep_enable_timer_wakeup((nextSendTime) * 10e2);
    esp_deep_sleep_start();
}
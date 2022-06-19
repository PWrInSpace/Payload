#include "../include/com/functions.h"
#include "../include/structs/PayloadControl.h"

// To implementation of each module:
extern PayloadControl payload;


void initPeripherals()
{
}

/**********************************************************************************************/

// To implementation of each module:

void measure()
{
}

/**********************************************************************************************/

// To implementation of each module:

void rxNowHandler(const uint8_t *incomingData, int len)
{
    memcpy((void *)&dataFromObc, (DataFromObc *)incomingData, sizeof(dataFromObc));
    Serial.print("COMMAND: ");
    Serial.println(dataFromObc.command);
    Serial.print("COMMAND TIME: ");
    Serial.println(dataFromObc.commandTime);

    if (xQueueSend(payload.hardware.espNowQueue, (void *)&dataFromObc, 0) != pdTRUE)
    {
        Serial.print("err data rxnow queue send\n");
    }
}
    /**********************************************************************************************/
    /**********************************************************************************************/
    /**********************************************************************************************/

    uint32_t getPowerTime_ms()
    {

        timeval tv;
        timezone tz;

        gettimeofday(&tv, (void *)&tz);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }

    /**********************************************************************************************/

    void goToSleep()
    {
        // dataToObc.wakenUp = false;
        esp_sleep_enable_timer_wakeup((sleepTime)*10e2);
        esp_deep_sleep_start();
    }


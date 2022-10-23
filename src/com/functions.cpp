#include "../include/com/functions.h"
#include "../include/structs/PayloadControl.h"
#include "../../include/hardware/RPiControl.h"
#include "../../include/tasks/tasks.h"
// To implementation of each module:
extern PayloadControl payload;

void initPeripherals()
{
    while (!imuInit())
    {
        Serial.println("Imu not initialized!");
    }

    payload.hardware.imu.begin();
}

/**********************************************************************************************/

// To implementation of each module:

void measure()
{
    // dataTask();
    //hehe dzielnik 10k || 10k 
    // vbat = read*2
    payload.vbat = float(analogRead(BATT_CHECK) * VOLT_DIV_SCALER);
    Serial.printf("voltage measure: [V]%f \n", payload.vbat);
    dataToSD.vBat = payload.vbat;
    dataToObc.vBat = payload.vbat;
    // Serial.println((void*)dataToSD);
    if (xQueueSend(payload.hardware.sdDataQueue, (void *)&dataToSD, 0) != pdTRUE)
    {
        Serial.print("err data sd queue send\n");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
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

    if (dataFromObc.command == 21) // reset
    {
        RPiControl::raspberryOff();
        vTaskDelay(1500);
        RPiControl::raspberryPower();
    }

    if (dataFromObc.command == 37) // turn on
    {
        RPiControl::recordOn();
        RPiControl::ledOn();
    }

    if (dataFromObc.command == 69) // record off
    {
        RPiControl::recordOff();
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
    if(payload.isRecording == true)
    {
        RPiControl::recordOff();
        while(payload.isRecording == true){
            vTaskDelay( 500 / portTICK_PERIOD_MS);
        }
        payload.isRecording = false;
        dataToObc.isRecording = false;
        dataToSD.isRecording = false;
        vTaskDelay(10000/ portTICK_PERIOD_MS);
    }
    esp_sleep_enable_timer_wakeup((payload.nextSendTime) * 10e2);
    esp_deep_sleep_start();
}

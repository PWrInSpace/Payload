#include "../../include/tasks/tasks.h"
#include "../../include/hardware/RPiControl.h"

extern PayloadControl payload;

void rxHandlingTask(void *arg)
{
   
    Serial.println("rxHandlingtask works");
    while (1)
    {
        // ESPNOW
        if (xQueueReceive(payload.hardware.espNowQueue, (void *)&dataFromObc, 0) == pdTRUE)
        {
            Serial.print("ESP NOW: ");
            Serial.print(dataFromObc.command);
            payload.state = dataFromObc.state;

            switch (payload.state)
            {
            case INIT:
                payload.state = IDLE;
                break;

            case IDLE:
                sleepTime = 30000;
                goToSleep();
                break;

            case ARMED:
                sleepTime = 5000;
                goToSleep();
                break;

            case RDY_TO_LAUNCH:
                RPiControl::raspberryPower();
                break;

            case COUNTDOWN:
                break;

            case FLIGHT:
                break;

            case ON_GROUND:
                sleepTime = 60000;
                goToSleep();
                break;

            case HOLD:
                sleepTime = 60000;
                goToSleep();
                break;

            case ABORT:
                sleepTime = 60000;
                goToSleep();
                break;

            default:
                Serial.print("state unusable\n");
                Serial.print(payload.state);
                break;
            }
        }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
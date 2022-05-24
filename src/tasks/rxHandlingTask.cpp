#include "../../include/tasks/tasks.h"

void rxHandlingTask(void *arg)
{
    while (1)
    {
        Serial.println("rxHandlingtask works");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
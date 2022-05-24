#include "../../include/tasks/tasks.h"

void dataTask(void *arg)
{
    while(1)
    {
        Serial.println("Datatask works");
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}
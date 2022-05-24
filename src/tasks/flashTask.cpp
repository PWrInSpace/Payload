#include "../../include/tasks/tasks.h"

void flashTask(void *arg)
{
    while (1)
    {
        Serial.println("Flashtask works");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
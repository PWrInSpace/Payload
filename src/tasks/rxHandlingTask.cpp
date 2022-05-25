#include "../../include/tasks/tasks.h"
#include "../../include/hardware/RPiControl.h"
void rxHandlingTask(void *arg)
{

    RPiControl::init();
    while (1)
    {
        Serial.println("rxHandlingtask works");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
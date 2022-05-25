#include "../include/tasks/tasks.h"
#include "../include/config.h"
#include "../include/hardware/SDcard.h"

void sdTask(void *arg)
{
    SDCard mySD(payload.hardware.mySPI, SD_CS);
    char data[SD_FRAME_ARRAY_SIZE] = {};
    String dataPath = dataFileName;
    String logPath = logFileName;
    uint8_t sd_i = 0;
    vTaskDelay(500 / portTICK_RATE_MS);
    
    xSemaphoreTake(payload.hardware.spiMutex, pdTRUE);
    Serial.println("CHUJ");
    while (!mySD.init())
    {
        Serial.println("SD INIT ERROR!"); // DEBUG

        xSemaphoreGive(payload.hardware.spiMutex);

        vTaskDelay(500 / portTICK_PERIOD_MS);
        xSemaphoreTake(payload.hardware.spiMutex, pdTRUE);
    }
    while (mySD.fileExists(dataPath + String(sd_i) + ".txt"))
    {
        sd_i++;
    }
    dataPath = dataPath + String(sd_i) + ".txt";
    logPath = logPath + String(sd_i) + ".txt";
    xSemaphoreGive(payload.hardware.spiMutex);
    while (1)
    {

        if (xQueueReceive(payload.hardware.sdDataQueue, (void *)&data, 0) == pdTRUE)
        {

            xSemaphoreTake(payload.hardware.spiMutex, portMAX_DELAY);

            if (strncmp(data, "LOG", 3) == 0)
            {
                if (!mySD.write(logPath, data))
                {
                   
                }
            }
            else
            {
                if (!mySD.write(dataPath, data))
                {
                   
                }
            }
            xSemaphoreGive(payload.hardware.spiMutex);
        }
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}
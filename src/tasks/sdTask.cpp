#include "../include/tasks/tasks.h"
#include "Arduino.h"
#include "../include/hardware/SDcard.h"

void sdTask(void *arg) {

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    SPIClass SPISD(HSPI);
    //tutaj wybrac poprawne piny na SD

    SPISD.begin(GPIO_NUM_12, GPIO_NUM_27, GPIO_NUM_15);
    SPI.setClockDivider(SPI_CLOCK_DIV2);

    if (!SD.begin(SD_CS, SPISD)) { 
        Serial.println("Could not initialize SD card!!!\n");  
    }

    while (1)
    {
        // while (queue.getNumberOfElements() > 0)
        // {

        //     String dataFrame = queue.pop();

        //     SD_write("/R4_pitot.txt", dataFrame);

        //     vTaskDelay(1 / portTICK_PERIOD_MS);
        // }


        //TUTAJ DODAC ALBO KOLEJKE ALBO COS Z CZEGO BĘDĄ DANE ZAPISYWANE NA KARTE

        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
}
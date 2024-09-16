#include "functions.h"

void setup() {

    Serial.begin();
    Serial.setTimeout(10);
    vTaskDelay(100);

    // Inits:
    SPIFFS.begin(true);
    uint32_t totalBytes = SPIFFS.totalBytes();
    uint32_t usedBytes = SPIFFS.usedBytes();
    Serial.println("Free space (MB):");
    Serial.println((totalBytes - usedBytes)/1000000.0);

    pinMode(LED_PIN, OUTPUT);
    blinkNTimes(1);

    // RTOS ques and tasks:
    glob.dataFramesFifo = xQueueCreate(FRAMES_IN_QUE, sizeof(Frame));
    xTaskCreate((TaskFunction_t) uartCommTask, "Uart Comm Task",  16384, NULL, 1, NULL);
    //xTaskCreate((TaskFunction_t) flashTask,    "Flash Task",      65536, NULL, 5, NULL);

    // Writing mode:
    vTaskDelay(3000);
    if (isWritingMode()) {

        xTaskCreate((TaskFunction_t) writeData, "Write Data Task",  65536, NULL, 20, NULL);
        //writeData();
    }

    // Reading mode:
    else {

        readData();
    }
}

void loop() {
    vTaskDelay(10);
}

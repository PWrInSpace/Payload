#include "sdLogger.h"

// For use of static attribute:
QueueHandle_t SdLogger::sdQueue = {};

bool SdLogger::sdWrite(const String &path, const String &dataFrame) {

    File file = SD.open(path, "a");
    
    if(file) {
        if(!file.write((uint8_t *) dataFrame.c_str(), dataFrame.length())) {
            file.close();
            return false;
        }
    }     
    else return false;

    file.close();

    return true;
}

/**********************************************************************************************/

void SdLogger::begin() {

    sdQueue = xQueueCreate(QUE_SIZE, sizeof(DataToSave));
    xTaskCreate((TaskFunction_t) logTask, "SD Task", 65536, NULL, 2, NULL);
    
}

/**********************************************************************************************/

void SdLogger::push() {

    xQueueSend(sdQueue, (uint8_t*) &moduleData.dataToSave, 100);

}

String getFileName() {
    uint32_t number = 1;

    String name = String( "/data-" ) + String( number ) + String( ".txt" );

    while(SD.exists(name)){
        number++;
        name = String( "/data-" ) + String( number ) + String( ".txt" );
    }

    return name;
}

/**********************************************************************************************/

void SdLogger::logTask() {

    vTaskDelay(100 / portTICK_PERIOD_MS);

    SPIClass SPISD(HSPI);

    SPISD.begin(SCLK_PIN, MISO_PIN, MOSI_PIN);
    SPI.setClockDivider(SPI_CLOCK_DIV2);

    if (!SD.begin(SD_CS_PIN, SPISD)) {
        lg.error("sdLogger SD card was not initialized properly", "sd");
    }

    String fname = getFileName();

    while (1) {
        while (uxQueueMessagesWaiting(sdQueue) > 0) {
            DataToSave data;

            xQueueReceive(sdQueue, (uint8_t *)&data, portMAX_DELAY);
            String tempDataStr = data.toString();

            if (!sdWrite(fname, tempDataStr)) {
                lg.error("sdLogger write error", "sd");
            }

            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
    
}
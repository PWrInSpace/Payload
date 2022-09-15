#include "../include/tasks/tasks.h"
#include "../include/config.h"
#include "../include/hardware/SDcard.h"

void sdTask(void *arg)
{
    SDCard mySD(payload.hardware.mySPI, SD_CS);
    DataToSD data;
    String dataPath = dataFileName;
    String logPath = logFileName;
    uint16_t sd_i = 0;

    char frameToSD[SD_FRAME_ARRAY_SIZE] = {};
    // char shortFrameToSD[SD_FRAME_ARRAY_SIZE] = {};

    vTaskDelay(500 / portTICK_RATE_MS);
    xSemaphoreTake(payload.hardware.spiMutex, pdTRUE);
    Serial.println("Initializing sd task");
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
        // if (xQueueReceive(payload.hardware.sdDataQueue, (void *)&data, 0) == pdTRUE)
        // {
        // snprintf(shortFrameToSD, sizeof(shortFrameToSD), "time: %5l, vbat: %f, recording: %d, isRPiOn: %d,\n",
        //          millis(), data.vBat, data.isRecording, data.isRpiOn);
        data.imuData = payload.hardware.imu.getDataStruct();
        snprintf(frameToSD, sizeof(frameToSD), "vbat: %f, recording: %d, ax: %0.3f, ay: %0.3f, az: %0.3f, gx: %0.3f, gy: %0.3f, gz: %0.3f, mx: %0.3f, my: %0.3f, mz: %0.3f, press: %0.3f, alt: %0.3f, tempr: %0.3f\n",
                 millis(), data.vBat, data.isRecording,
                 data.imuData.ax, data.imuData.ay, data.imuData.az,
                 data.imuData.gx, data.imuData.gy, data.imuData.gz,
                 data.imuData.mx, data.imuData.my, data.imuData.mz,
                 data.imuData.pressure, data.imuData.altitude, data.imuData.temperature);

        // Serial.println(shortFrameToSD);
        xSemaphoreTake(payload.hardware.spiMutex, portMAX_DELAY);
        if (!mySD.write(dataPath, frameToSD))
        {
            Serial.println("could not save in given path");
        }
        xSemaphoreGive(payload.hardware.spiMutex);
        // }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
#include "../../include/tasks/tasks.h"

LSM6 accel;
LIS3MDL mag;
LPS ps;

bool imuInit()
{
    if (!accel.init())
    {
        return 0;
    }
    if (!mag.init())
    {
        return 0;
    }
    if (!ps.init())
    {
        return 0;
    }
    accel.enableDefault();
    mag.enableDefault();
    ps.enableDefault();
    return 1;
}

void dataTask(void *arg)
{
    float vBat = 0.0;

    while (!imuInit())
    {
        Serial.println("IMU sra");
    }
    
    while (1)
    {
        Serial.println("Datatask works");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
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

void dataTask()
{
    // dataToSD.imuData = payload.hardware.imu.getDataStruct();
    // Serial.println("Datatask works");
    
}
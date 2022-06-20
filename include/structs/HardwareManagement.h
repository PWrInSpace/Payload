#ifndef HARDWARE_MANAGEMENT_HH
#define HARDWARE_MANAGEMENT_HH
#include "pinout.h"
#include "config.h"
#include "SPI.h"
#include "Wire.h"
#include "../include/hardware/ImuAPI.h"

struct HardwareManagement
{
    // tasks
    TaskHandle_t sdTask;
    // queues
    QueueHandle_t sdDataQueue;
    // mutexes
    SemaphoreHandle_t spiMutex;
    SemaphoreHandle_t i2cMutex;
    // peripherals
    SPIClass mySPI;
    TwoWire myI2C;
    // imu;
    ImuAPI imu;

    HardwareManagement(): 
    
    sdTask(NULL),
    sdDataQueue(NULL),
    spiMutex(NULL),
    i2cMutex(NULL),
    mySPI(SPIClass(HSPI)),
    myI2C(TwoWire(0)) {}

};

#endif
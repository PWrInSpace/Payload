#ifndef HARDWARE_MANAGEMENT_HH
#define HARDWARE_MANAGEMENT_HH
#include "pinout.h"
#include "config.h"
#include "SPI.h"
#include "Wire.h"

struct HardwareManagement
{
    // tasks
    TaskHandle_t rxHandlingTask;
    TaskHandle_t dataTask;
    TaskHandle_t sdTask;
    TaskHandle_t flashTask;

    // queues
    QueueHandle_t sdDataQueue;
    QueueHandle_t sdErrorsQueue;
    QueueHandle_t flashQueue;
    QueueHandle_t espNowQueue;

    // mutexes
    SemaphoreHandle_t spiMutex;
    SemaphoreHandle_t i2cMutex;

    // peripherals
    SPIClass mySPI;
    TwoWire myI2C;

    HardwareManagement() : 
    rxHandlingTask(NULL),
    dataTask(NULL),
    sdTask(NULL),
    flashTask(NULL),
    sdDataQueue(NULL),
    sdErrorsQueue(NULL),
    flashQueue(NULL),
    espNowQueue(NULL),
    spiMutex(NULL),
    i2cMutex(NULL),
    mySPI(SPIClass(HSPI)),
    myI2C(TwoWire(0)) {}
};

#endif
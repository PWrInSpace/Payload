#ifndef PAYLOAD_CONTROL_HH
#define PAYLOAD_CONTROL_HH

#include "../include/pinout.h"
#include "../include/config.h"
#include "../structs/HardwareManagement.h"

// QUEUE LENGTHS
#define SD_QUEUE_LENGTH 10
#define FLASH_QUEUE_LENGTH 20 
#define ESP_NOW_QUEUE_LENGTH 5

struct PayloadControl
{
    HardwareManagement hardware;
    PayloadControl() = default;
};

#endif
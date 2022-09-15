#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H
#include "../include/hardware/ImuStructs.h"

struct DataToObc {
    bool wakenUp : 1; //if waken up
    bool isRecording = false; // if rpi says it is
    bool data : 1; // if data is collected
    float vBat = 21.37; //battery voltage
    bool isRpiOn = false;
};

struct DataFromObc {
    uint8_t command;
    uint8_t commandTime; 
};

struct DataToSD{
    bool wakenUp : 1; //if waken up
    bool isRecording = false; // if rpi says it is
    bool data : 1; // if data is collected
    float vBat = 21.37; //battery voltage
    bool isRpiOn = false;
    ImuData imuData;
};

extern volatile DataToObc dataToObc;
extern volatile DataFromObc dataFromObc;
extern volatile DataToSD dataToSD;
extern volatile uint16_t sleepTime;

#define MIN_SLEEP_TIME_MS 2000

#endif


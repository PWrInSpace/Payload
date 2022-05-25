#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

struct DataToObc {

    bool wakenUp : 1; //if waken up
    bool isRecording : 1; // if rpi says it is
    bool data : 1; // if data is collected
    float vBat = 0.0; //battery voltage
    // To implementation of each module.
};

struct DataFromObc {

    uint8_t command;
    uint8_t commandTime;
};

extern volatile DataToObc dataToObc;
extern volatile DataFromObc dataFromObc;
extern volatile uint16_t nextSendTime;

#define MIN_SLEEP_TIME_MS 2000

#endif
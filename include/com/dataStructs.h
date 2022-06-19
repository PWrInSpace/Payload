#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

struct DataToObc {
    bool wakenUp : 1; //if waken up
    bool isRecording : 1; // if rpi says it is
    bool data : 1; // if data is collected
    // To implementation of each module.
};

struct DataFromObc {
    uint8_t command;
    uint8_t commandTime;
    uint8_t state;
};

struct DataToSD{
    bool wakenUp : 1; //if waken up
    float vBat = 21.37; //battery voltage
    uint8_t state;
};

extern volatile DataToObc dataToObc;
extern volatile DataFromObc dataFromObc;
extern volatile DataToSD dataToSD;
extern volatile uint16_t sleepTime;

#define MIN_SLEEP_TIME_MS 2000

#endif


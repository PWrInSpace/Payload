#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdint.h>

struct Globals {

    volatile uint8_t rotcketState = 0;
    QueueHandle_t dataFramesFifo;
};

#define DATA_IN_FRAME 10000


struct Frame {

    uint32_t microsStart;
    uint8_t adcX[DATA_IN_FRAME];
    uint8_t adcY[DATA_IN_FRAME];
    uint8_t adcZ[DATA_IN_FRAME];
    uint8_t state;
};

extern Globals glob;

#endif

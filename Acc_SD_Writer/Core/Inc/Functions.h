#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "adc.h"
#include <string.h>

#define ADC_SIZE 340
#define QUE_SIZE 40
#define ADC_NO 3

typedef struct {

    uint32_t time;
    uint16_t adc[ADC_SIZE][ADC_NO];
    uint8_t state;

} Frame;

extern uint32_t us_time;

void doMeasurements(Frame* frame);

#endif /* INC_FUNCTIONS_H_ */

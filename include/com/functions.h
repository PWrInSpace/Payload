#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Arduino.h"
#include <sys/time.h>
#include "dataStructs.h"

// To implementation:
void initPeripherals();
void measure();
void rxNowHandler(const uint8_t *incomingData, int len);

// Not to touch:
uint32_t getPowerTime_ms();
void goToSleep();

#endif
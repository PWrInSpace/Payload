#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Arduino.h"
#include <sys/time.h>
#include "dataStructs.h"
#include "now.h"
#include "localComms.h"
#include "logger.h"

class Functions {

    // Not to touch:
    uint32_t getPowerTime_ms();

public:

    // To implementation:
    void initPeripherals();
    void measure();
    void internalStateManagement();
    
    void rxNowHandler(const uint8_t *incomingData, int len);
    float getVBatt();
    bool sleepBlockedByPayloadState();
    bool wakenUpStatus();
    void detectionPowerMgmt();

    void changePayloadStateToReady();
    void changePayloadStateToAwait();
    void changePayloadStateToAwaitOnGround();
    void changePayloadStateToWrite();

    // Not to touch:
    void goToSleep();
    bool stateInWakenUp();
    void checkSleepThings();
};

extern Functions fcn;

#endif
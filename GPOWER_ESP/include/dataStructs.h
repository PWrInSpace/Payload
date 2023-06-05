#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#define ANSWER_WAIT_MS 750
#define MIN_SLEEP_TIME_MS 2000
#define SLEEP_TIME_MS 30000
#define PLD_STATE_CH_DELAY 10000
#define GROUND_GRACE_PERIOD_WRITE 300000
#define GROUND_GRACE_PERIOD_READY 60000

#define IDNAME(name) #name

#include "pinout.h"
#include "stdint.h"
#include "Arduino.h"

// IMPORTANT! To set for each module and state:
enum Periods {

    INIT_PERIOD             = SLEEP_TIME_MS, // DETECTION POWER OFF
    IDLE_PERIOD             = SLEEP_TIME_MS, // DETECTION POWER OFF
    RECOVERY_ARM_PERIOD     = SLEEP_TIME_MS, // DETECTION POWER OFF
    FUELING_PERIOD          = SLEEP_TIME_MS, // DETECTION POWER OFF
    ARMED_TO_LAUNCH_PERIOD  = 2000, // AWAIT
    RDY_TO_LAUNCH_PERIOD    = 1000, // READY
    COUNTDOWN_PERIOD        = 1000, // WRITE
    FLIGHT_PERIOD           = 1000, // WRITE
    FIRST_STAGE_REC_PERIOD  = 1000, // WRITE
    SECOND_STAGE_REC_PERIOD = 1000, // WRITE
    ON_GROUND_PERIOD        = SLEEP_TIME_MS, // WRITE for 360 sec -> READY for 5 sec -> AWAIT and SLEEP
    HOLD_PERIOD             = 2000, // CONTINUE LAST PAYLOAD STATE (No payload state change)
    ABORT_PERIOD            = 2000, // if WRITE then WRITE for 5 sec -> READY for 5 sec -> AWAIT
};

// IMPORTANT END!

enum States {
  INIT = 0,
  IDLE,
  RECOVERY_ARM,
  FUELING,
  ARMED_TO_LAUNCH,
  RDY_TO_LAUNCH,
  COUNTDOWN,
  FLIGHT,
  FIRST_STAGE_RECOVERY,
  SECOND_STAGE_RECOVERY,
  ON_GROUND,
  HOLD,
  ABORT,
  NO_CHANGE = 0xff //DO NOT USE, ONLY FOR REQUEST PURPOSE
};

static const char* statesNames[] = {
    IDNAME(INIT),
    IDNAME(IDLE),
    IDNAME(RECOVERY_ARM),
    IDNAME(FUELING),
    IDNAME(ARMED_TO_LAUNCH),
    IDNAME(RDY_TO_LAUNCH),
    IDNAME(COUNTDOWN),
    IDNAME(FLIGHT),
    IDNAME(FIRST_STAGE_RECOVERY),
    IDNAME(SECOND_STAGE_RECOVERY),
    IDNAME(ON_GROUND),
    IDNAME(HOLD),
    IDNAME(ABORT),
    IDNAME(NO_CHANGE)
};

/*
    Payload internal state
*/
enum PayloadInternalState {
    EMPTY = 0,
    AWAIT,
    READY,
    WRITE,
};

static const char* payloadInternalStateNames[] = {
    IDNAME(EMPTY),
    IDNAME(AWAIT),
    IDNAME(READY),
    IDNAME(WRITE)
};

struct DataToSave {

    // IMPORTANT! To implementation of each module:

    String toString() {

        char outStr[50];
        sprintf(outStr, "test%d", 1);
        return String(outStr);
    }
    // IMPORTANT END!
};

struct DataToObc {

    bool wakenUp : 1;
    float batteryVoltage;

};

struct DataFromObc {

    uint32_t commandNum;
    int32_t commandArg;
};

struct ModuleData {

    DataToObc dataToObc;
    DataFromObc dataFromObc;
    DataToSave dataToSave;

    uint8_t payloadState = AWAIT;
    uint8_t obcState = INIT;

    // IMPORTANT! Set the flash or sd logger:
    const bool flashSave = false;
    const bool sdSave = false;
    // IMPORTANT END!

    bool writeStopInProgress = false;
    unsigned long millisFromChangeWriteState = 0;

    bool readyStopInProgress = false;
    unsigned long millisFromChangeReadyState = 0;

    uint16_t stateTimes[13] = {
        INIT_PERIOD, IDLE_PERIOD, RECOVERY_ARM_PERIOD,
        FUELING_PERIOD, ARMED_TO_LAUNCH_PERIOD, RDY_TO_LAUNCH_PERIOD,
        COUNTDOWN_PERIOD, FLIGHT_PERIOD, FIRST_STAGE_REC_PERIOD,
        SECOND_STAGE_REC_PERIOD, ON_GROUND_PERIOD,
        HOLD_PERIOD, ABORT_PERIOD};

    // IMPORTANT! Add other global variables in this struct:

    // IMPORTANT END!
};

extern volatile ModuleData moduleData;

#endif
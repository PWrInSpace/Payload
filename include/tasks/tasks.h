#ifndef TASKS_HH
#define TASKS_HH

// #include <LITTLEFS.h>
// #include <LoRa.h>
// #include <SparkFun_u-blox_GNSS_Arduino_Library.h>

// #include "../include/structs/rocketControl.h"
// #include "../include/structs/dataStructs.h"
// #include "../include/timers/watchdog.h"
#include "../include/com/now.h"
#include "pinout.h"
#include "../hardware/SDcard.h"
#include "../structs/errors.h"
#include "../structs/PayloadControl.h"
#include "../../include/hardware/ImuAPI.h"
// #include "../include/structs/stateMachine.h"

extern PayloadControl payload;

//Tasks
void rxHandlingTask(void *arg);

void stateTask(void *arg);
void dataTask(void *arg);
void sdTask(void *arg);
void flashTask(void *arg);


//Timersandvoid watchdogTimerCallback(TimerHandle_t xTimer);
void disconnectTimerCallback(TimerHandle_t xTimer);

// Functions
void parseR4A(char* data);
void parseR4O(char* data);

#endif
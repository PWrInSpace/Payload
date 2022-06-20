#ifndef PAYLOAD_CONTROL_HH
#define PAYLOAD_CONTROL_HH

#include "../include/pinout.h"
#include "../include/config.h"
#include "../structs/HardwareManagement.h"
#include "../hardware/ImuAPI.h"

// QUEUE LENGTHS
#define SD_QUEUE_LENGTH 10
#define FLASH_QUEUE_LENGTH 20 
#define ESP_NOW_QUEUE_LENGTH 5

enum States
{
  INIT = 0,
  IDLE,
  FUELING,
  ARMED,
  RDY_TO_LAUNCH,
  COUNTDOWN,
  FLIGHT,
  FIRST_STAGE_RECOVERY,
  SECOND_STAGE_RECOVERY,
  ON_GROUND,
  HOLD,
  ABORT,
  NO_CHANGE = 0xff // DO NOT USE, ONLY FOR REQUEST PURPOSE
};

struct PayloadControl
{
    HardwareManagement hardware;
    PayloadControl() = default;
    uint16_t nextSendTime = 1000;
    bool isRecording = false;
    bool isRPiOn = false;
    float vbat = 0.0;

};



#endif
#ifndef CONFIG_HH
#define CONFIG_HH

#include "Arduino.h"
#include "FreeRTOS.h"
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
// #include "../include/structs/stateMachine.h"  //bedzie raczej potrzebne prymitywne chociaz do eykrycia startu

#define SD_FRAME_ARRAY_SIZE 512
#define LORA_FRAME_ARRAY_SIZE 256
#define LORA_TX_DATA_PREFIX "R4D;"
#define LORA_TX_OPTIONS_PREFIX "R4C;"

//PERIODS
#define ESP_NOW_SLEEP_TIME 30 * 1000
#define ESP_NOW_LONG_PERIOD 500
#define ESP_NOW_SHORT_PERIOD 100
#define LORA_PERIOD 500
#define DATA_PERIOD 100
#define FLASH_SHORT_PERIOD 100
#define FLASH_LONG_PERIOD 500
#define SD_SHORT_PERIOD 100
#define SD_LONG_PERIOD 500
#define IDLE_PERIOD 10 * 1000

// //ESP NOW NOTIFICATION 
// #define TANWA 1
// #define PITOT 2
// #define MAIN_VALVE 3
// #define UPUST_VALVE 4
// #define BLACK_BOX 5
// #define PAYLOAD 6

static String dataFileName= "/data_";
static String logFileName = "/log_";

//TIMERS
static const TickType_t watchdogDelay = 1000 / portTICK_PERIOD_MS; //250ms
static const TickType_t disconnectDelay = 0.5 * 60 * 1000 / portTICK_PERIOD_MS; //15min


#endif
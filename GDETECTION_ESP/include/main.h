#include <Arduino.h>
#include "pinout.h"
#include "functions.h"

#include "RTOS.h"
#include "SPI.h"
#include "sd_diskio.h"
#include "SD.h"
#include <FS.h>
#include "localComms.h"
#include "logger.h"
#include <esp_task_wdt.h>

volatile ModuleData moduleData;
Functions fcn;
LocalComms lc;
Logger lg;

void setup();
void loop();
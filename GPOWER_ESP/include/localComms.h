#ifndef LOCALCOMMS_H
#define LOCALCOMMS_H

#include "Arduino.h"
#include <Wire.h>
#include "pinout.h"
#include "dataStructs.h"
#include "logger.h"

bool monitor();

void sendStatus();

//extern State currentState;

#endif
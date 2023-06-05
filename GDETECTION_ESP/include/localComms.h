#ifndef LOCALCOMMS_H
#define LOCALCOMMS_H

#include "Arduino.h"
#include <Wire.h>
#include "pinout.h"
#include "dataStructs.h"
#include "logger.h"

class LocalComms {

public:
    bool monitor();
    
    static void receiveEvent(int);
    static void requestEvent();
};

extern LocalComms lc;

#endif
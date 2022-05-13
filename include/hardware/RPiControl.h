#ifndef RPI_CONTROL_H
#define RPI_CONTROL_H
#include <esp32-hal.h>
#include <string>
#include <HardwareSerial.h>
#include <map>
#include "../pinout.h"

class RPiControl
{

public:
    static void recordOn();
    static void recordOff();
    static void raspberryPower();
    static void raspberryOff();
    static void ledPower();
    static void ledOff();
    static void init();
    static void resolveCommand(std::string command);

     enum RPiCommands
    {
        CAM_REC = 0,
        CAM_OFF = 1,
        RPI_ON = 2,
        RPI_OFF = 3,
    } ;

private:
   
    RPiControl() {}
};

#endif

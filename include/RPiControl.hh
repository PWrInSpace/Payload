#include <esp32-hal.h>
#include <string>
#include <HardwareSerial.h>
#include <map>

#define RPI_PIN_16 5
#define RPI_PIN_18 18
#define RPI_PIN_22 19
#define RPI_POWER_PIN 23

#ifndef RASPBERRY_FUNC_HANDLER_LIB
#define RASPBERRY_FUNC_HANDLER_LIB

class RPiControl
{

public:
    static void recordOn();
    static void recordOff();
    static void raspberryPower();
    static void raspberryOff();
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

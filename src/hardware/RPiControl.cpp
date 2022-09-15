#include <../include/hardware/RPiControl.h>
#include <stdexcept>
#include "../include/ble/ESP32_blelib.h"
#include <stdio.h>
#include <string>

using namespace std;

void RPiControl::resolveCommand(std::string command)
{
    String msg;
    int i = -1;
    try
    {
        i = RPiCommands(atoi(command.c_str())); // stoi cos nie działa
    }
    catch (std::invalid_argument const &ex)
    {
        Serial.println("A string with no command detected!\n");
    }

    switch (i)
    {
    case CAM_REC:
    {
        msg = "CAM is on";
        RPiControl::recordOn();
        RPiControl::ledPower();
        break;
    }
    case CAM_OFF:
    {
        msg = "CAM is off";
        RPiControl::recordOff();
        RPiControl::ledOff();
        break;
    }
    case RPI_ON:
    {
        msg = "RPI on";
        RPiControl::raspberryPower();
        break;
    }
    case RPI_OFF:
    {
        msg = "RPI off";
        RPiControl::raspberryOff();
        break;
    }
    case WRNG_COMMND:
    {
        break;
    }
    default:
        msg = "WRONG COMMAND";
        break;
    }
    // ESP32_blelib::respondOnCharac(msg);
    msg += " rpi_control\n";
    Serial.println(msg.c_str());
}

void RPiControl::init()
{
    digitalWrite(RPI_PIN_16, LOW);
    // digitalWrite(RPI_PIN_22, LOW);
    digitalWrite(RPI_POWER_PIN, LOW);
    digitalWrite(WAKE_LED, LOW);
}

void RPiControl::recordOn()
{
    digitalWrite(RPI_PIN_16, HIGH);
}

void RPiControl::recordOff()
{
    if (digitalRead(RPI_PIN_16) == 1)
    {
        digitalWrite(RPI_PIN_16, LOW);
    }
}

void RPiControl::raspberryPower()
{
    // if (digitalRead(RPI_POWER_PIN) == 0) //co to jest
    // {
    digitalWrite(RPI_POWER_PIN, HIGH);
    // }
}
void RPiControl::raspberryOff()
{
    // if (digitalRead(RPI_POWER_PIN) == 1) //co to jest
    // {
    digitalWrite(RPI_POWER_PIN, LOW);
    // }
}

void RPiControl::ledPower()
{
    if (digitalRead(WAKE_LED) == 0)
    {
        digitalWrite(WAKE_LED, HIGH);
    }
}
void RPiControl::ledOff()
{
    if (digitalRead(WAKE_LED) == 1)
    {
        digitalWrite(WAKE_LED, LOW);
    }
}
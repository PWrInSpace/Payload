#include <../include/hardware/RPiControl.h>

void RPiControl::resolveCommand(std::string command)
{
    int i;

    i = (RPiCommands)3;

    switch (i)
    {
    case CAM_REC:
    {
        Serial.println("CAM is on");
        RPiControl::recordOn();
        RPiControl::ledPower();
        break;
    }
    case CAM_OFF:
    {
        Serial.println("CAM is off");
        RPiControl::recordOff();
        RPiControl::ledOff();
        break;
    }
    case RPI_ON:
    {
        Serial.println("RPI on");
        RPiControl::raspberryPower();
        break;
    }
    case RPI_OFF:
    {
        Serial.println("RPI off");
        RPiControl::raspberryOff();
        break;
    }
    default:
        Serial.println("WRONG COMMAND");
        break;
    }
}

void RPiControl::init()
{
    pinMode(RPI_PIN_16, OUTPUT);
    pinMode(RPI_PIN_18, INPUT);
    pinMode(RPI_PIN_22, OUTPUT);
    pinMode(RPI_POWER_PIN, OUTPUT);
    pinMode(WAKE_LED, OUTPUT);

    digitalWrite(RPI_PIN_16, LOW);
    digitalWrite(RPI_PIN_22, LOW);
    digitalWrite(RPI_POWER_PIN, LOW);
    digitalWrite(WAKE_LED, LOW);
}

void RPiControl::recordOn()
{
    if (digitalRead(RPI_PIN_16) == 0)
    {
        digitalWrite(RPI_PIN_16, HIGH);
    }
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
    if (digitalRead(RPI_POWER_PIN) == 0)
    {
        digitalWrite(RPI_POWER_PIN, HIGH);
    }
}
void RPiControl::raspberryOff()
{
    if (digitalRead(RPI_POWER_PIN) == 1)
    {
        digitalWrite(RPI_POWER_PIN, LOW);
    }
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
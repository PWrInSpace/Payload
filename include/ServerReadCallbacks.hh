#include <BLEServer.h>
#include <BLEUtils.h>
#include <HardwareSerial.h>
#include <RPiControl.hh>

extern std::string message;
static bool messageReceivedComplete;

class ServerReadCallbacks : public BLECharacteristicCallbacks
{
public:
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue();
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
        {
            Serial.print(rxValue[i]);
        }
        Serial.println();

        message = rxValue;

        messageReceivedComplete = true;
        // respond(rxValue);
        // while (1)
        //     if (Serial2.availableForWrite())
        //     {
        //         Serial2.write(rxValue.c_str(), rxValue.length() + 1);
        //         Serial.println("wrote to tanwa");
        //         break;
        //     }

        if (rxValue == "CAM")
        {
            Serial.println("CAM WORK");
            RPiControl::recordOn();
        }
        if (rxValue == "LOW")
        {
            Serial.println("piny niskie");
            digitalWrite(RPI_PIN_16, LOW);
        }
        if (rxValue == "RPIH")
        {
            Serial.println("RPI on");
            digitalWrite(23, HIGH);
        }
        if (rxValue == "RPIL")
        {
            Serial.println("RPI off");
            digitalWrite(23, LOW);
        }
    }

    // void callback(BLECharacteristic *pCharacteristic)
    // {
    //     pCharacteristic.setValue(send_message);
    //     pCharacteristic.notify();
    //     Serial.println("sent response");
    // }
};
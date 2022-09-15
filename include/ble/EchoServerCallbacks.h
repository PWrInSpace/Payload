#include <BLEServer.h>
#include <HardwareSerial.h>
static bool deviceConnected;

class EchoServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("Connected!");
        
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("Disconnected!");
        pServer->getAdvertising()->start();
        
    }
};
#include <../include/ble/ESP32_blelib.h>

void ESP32_blelib::init(BLECharacteristic *pCharacteristicTX, BLECharacteristic *pCharacteristicRX)
{
    Serial.println("Initializing BLE server");
    deviceConnected = false;
    messageReceivedComplete = false;

    BLEDevice::init("ESP32BLE");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(BLEUUID((uint16_t)0x1700));

    pServer->setCallbacks(new EchoServerCallbacks());
    pService->addCharacteristic(pCharacteristicTX);
    pService->addCharacteristic(pCharacteristicRX);

    pCharacteristicRX->addDescriptor(new BLE2902());
    pCharacteristicRX->setCallbacks(new ServerReadCallbacks());
    pCharacteristicRX->setValue("default");
    pCharacteristicTX->addDescriptor(new BLE2902());

    pServer->getAdvertising()->addServiceUUID(SERVICE_UUID);
    pService->start();
    pServer->getAdvertising()->start();
    Serial.println("Waiting for a Client to connect...");
}

void ESP32_blelib::respondOnCharac(BLECharacteristic *pCharacteristicRSPND, String value){
    pCharacteristicRSPND->setValue(value.c_str());

}
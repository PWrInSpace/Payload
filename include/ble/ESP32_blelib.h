#include <BLEDevice.h>
#include <BLE2902.h>
#include <string>
#include "../include/ble/EchoServerCallbacks.h"
#include "../include/ble/ServerReadCallbacks.h"

#ifndef BLUETOOTH_LE_READY_LIB
#define BLUETOOTH_LE_READY_LIB

#define SERVICE_UUID "00001111-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID_TX "c0de0002-feed-f00d-c0ff-eeb3d05ebeef"
#define CHARACTERISTIC_UUID_RX "c0de0003-feed-f00d-c0ff-eeb3d05ebeef"
#define LED_BUILTIN 2

extern std::string message;

static BLECharacteristic pCharacteristicRX(
    "00002222-0000-1000-8000-00805F9B34FB",
    BLECharacteristic::PROPERTY_WRITE_NR);

static BLECharacteristic pCharacteristicTX(
    "00007777-0000-1000-8000-00805F9B34FB",
    BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY);

class ESP32_blelib
{
public:
    static void init(BLECharacteristic *pCharacteristicTX, BLECharacteristic *pCharacteristicRX);
    static void respondOnCharac(BLECharacteristic *pCharacteristicRSPND,String value = "default");

private:
    ESP32_blelib() {}
};

#endif



#include <Arduino.h>
#include <FreeRTOS.h>
#include <SPI.h>
#include <Wire.h>
#include <BLEDevice.h>
#include "../include/ESP32_blelib.h"
#include "../include/hardware/RPiControl.h"
#include "../include/hardware/ImuAPI.h"
#include "../include/tasks/tasks.h"

ImuAPI IMU(AccelerometerScale::A_16g, GyroscpoeScale::G_1000dps);
SPIClass mySpi(HSPI);
Wire myWire(21, 22, 100E3);

std::string message;
// heheheh

void setup()
{
  Serial.begin(115200);
  mySPI.begin(SCK,MISO,MOSI);

  RPiControl::init(); // init raspberry controll first so the pins are set correctly before anything
  ESP32_blelib::init(&pCharacteristicTX, &pCharacteristicRX);

  if (!IMU.begin())
  {
    IMU.errors = IMU_INIT_ERROR;
    while (1)
    {
      delay(100);
    }
  }

  if (!IMU.setInitPressure())
  {
    IMU.errors = IMU_PRESSURE_ERROR;
  }
}

void loop()
{
  delay(20);
  IMU.readRawData();
  IMU.getData();
  // text = Serial2.readString();
  // Serial.println(text.c_str());
  // pCharacteristicTX.setValue(text.c_str());
  // pCharacteristicTX.notify();
  // // if device is connected and all packets are received, then respond
  // if (deviceConnected && messageReceivedComplete)
  // {
  //     messageReceivedComplete = false;
  //     if (String("hello").c_str() == message)
  //     {
  //         respond(String("world").c_str());
  //         Serial.println("sent world");
  //     }
  //     else
  //     {
  //         respond(message);
  //         Serial.println("echoed");
  //     }
  // }
}

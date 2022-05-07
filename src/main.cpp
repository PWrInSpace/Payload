#include <Arduino.h>
#include <BLEDevice.h>
#include <ESP32_blelib.hh>
#include <RPiControl.hh>
#include <errorStructs.h>
#include <imuAPI.h>

Errors errors;

ImuAPI IMU(AccelerometerScale::A_16g, GyroscpoeScale::G_1000dps);

std::string message;

void setup()
{
    Serial.begin(115200);
    RPiControl::init(); //init raspberry controll first so the pins are set correctly before anything
    ESP32_blelib::init(&pCharacteristicTX,&pCharacteristicRX);

     if(!IMU.begin()){
    errors.imu_error = IMU_INIT_ERROR;
    while (1)
    {
      delay(100);
    }
  }

  if (!IMU.setInitPressure())
  {
    errors.imu_error = IMU_PRESSURE_ERROR;
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

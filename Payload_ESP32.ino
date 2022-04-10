#include <LSM6.h>
#include <LPS.h>
#include <LIS3MDL.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#define spiPin 4

LSM6 accel;
LIS3MDL mag;
LPS ps;


File payloadData;
char data[100];

bool imuInit() {
  if (!accel.init())
  {
    return 0;
  }
  if (!mag.init())
  {
    return 0;
  }
  if (!ps.init())
  {
    return 0;
  }
  accel.enableDefault();
  mag.enableDefault();
  ps.enableDefault();
  return 1;
}


void setup() {
  Serial.begin(9600);
  Wire.begin();
  if (!imuInit) {
    Serial.println("IMU not detected");
  }
  SD.begin(spiPin);
}

void loop() {
  mag.read();
  accel.read();
  snprintf(data, sizeof(data), "A:%6d;%6d;%6d; G:%6d;%6d;%6d; M:%6d;%6d;%6d; P:%6f T:%6f",
           accel.a.x, accel.a.y, accel.a.z,
           accel.g.x, accel.g.y, accel.g.z,
           mag.m.x, mag.m.y, mag.m.z,
           ps.readPressureMillibars(), ps.readTemperatureC());
  Serial.println(data);
  payloadData = SD.open("payload_data", FILE_WRITE);
  if (payloadData) {
    payloadData.println(data);
    payloadData.close();
  }



}

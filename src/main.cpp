#include "../include/pinout.h"
#include "../include/config.h"
#include <BLEDevice.h>
#include <esp_wifi.h>
#include "../include/ble/ESP32_blelib.h"
#include "../include/hardware/RPiControl.h"
#include "../include/hardware/ImuAPI.h"
#include "../include/structs/PayloadControl.h"
#include "../include/tasks/tasks.h"
#include "../include/com/now.h"

PayloadControl payload;
std::string message;
uint32_t loopTimer;

volatile DataToObc dataToObc;
volatile DataFromObc dataFromObc;
DataToSD dataToSD;

void setup()
{
  Serial.begin(115200);
  loopTimer = 30000;
  payload.hardware.imu = ImuAPI(AccelerometerScale::A_16g, GyroscpoeScale::G_1000dps);
  // set mac adress
  WiFi.mode(WIFI_STA);
  esp_wifi_set_mac(WIFI_IF_STA, addressPayload);
  Serial.println(WiFi.macAddress());
  // initPeripherals(); //ogranac imu kurw

  nowInit();
  nowAddPeer(addressObc, 0);

  // ESP32_blelib::init(&pCharacteristicTX, &pCharacteristicRX);
  // RPiControl::init();
  /// queues
 payload.hardware.sdDataQueue = xQueueCreate(SD_QUEUE_LENGTH, sizeof(DataToSD));
  /// semaphores
  payload.hardware.spiMutex = xSemaphoreCreateMutex();
  payload.hardware.i2cMutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(sdTask, "SD task", 30000, NULL, 3, &payload.hardware.sdTask, APP_CPU_NUM);
}

void loop()
{
  measure();
  if(digitalRead(RPI_PIN_22)==HIGH){ //check if it is recording, RPI pin should go high else low
   dataToObc.isRecording = true;
   payload.isRecording = true;
   dataToSD.isRecording = true;
  }
  else{
    dataToObc.isRecording = false;
   payload.isRecording = false;
   dataToSD.isRecording = false;
  }
  if(digitalRead(RPI_PIN_18)==HIGH){ //check if programm initialized properly - pin should go high
    payload.isRPiOn = true;
  }

  if (millis() - loopTimer >= payload.nextSendTime)
  {
    Serial.println("sent to obc\n");
    loopTimer = millis();
    esp_now_send(addressObc, (uint8_t *)&dataToObc, sizeof(dataToObc));
  }

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

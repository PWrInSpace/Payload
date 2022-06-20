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


ImuAPI IMU(AccelerometerScale::A_16g, GyroscpoeScale::G_1000dps);
PayloadControl payload;
std::string message;

volatile DataToObc dataToObc;
volatile DataFromObc dataFromObc;
volatile DataToSD dataToSD;
volatile uint16_t sleepTime = 30000;

void setup()
{
  Serial.begin(115200);

    // set mac adress
  WiFi.mode(WIFI_STA);
   esp_wifi_set_mac(WIFI_IF_STA, adress);

  initPeripherals();

 nowInit();
   nowAddPeer(adressObc, 0);

  // ESP32_blelib::init(&pCharacteristicTX, &pCharacteristicRX);
  // RPiControl::init();
  /// queues
  payload.hardware.sdDataQueue = xQueueCreate(SD_QUEUE_LENGTH, sizeof(char[SD_FRAME_ARRAY_SIZE]));
  payload.hardware.sdDataQueue = xQueueCreate(SD_QUEUE_LENGTH, sizeof(char[SD_FRAME_ARRAY_SIZE]));
  payload.hardware.flashQueue = xQueueCreate(FLASH_QUEUE_LENGTH, sizeof(char[SD_FRAME_ARRAY_SIZE]));
  payload.hardware.espNowQueue = xQueueCreate(ESP_NOW_QUEUE_LENGTH, sizeof(uint8_t));

  /// semaphores
  payload.hardware.spiMutex = xSemaphoreCreateMutex();
  payload.hardware.i2cMutex = xSemaphoreCreateMutex();

  /// tasks
  // pro cpu
  xTaskCreatePinnedToCore(rxHandlingTask, "RX handling task", 8192, NULL, 2, &payload.hardware.rxHandlingTask, PRO_CPU_NUM);
  // app cpu
  // xTaskCreatePinnedToCore(dataTask, "Data task", 30000, NULL, 2, &payload.hardware.dataTask, APP_CPU_NUM);
  // xTaskCreatePinnedToCore(sdTask, "SD task", 30000, NULL, 3, &payload.hardware.sdTask, APP_CPU_NUM);
  xTaskCreatePinnedToCore(flashTask, "Flash task", 8192, NULL, 1, &payload.hardware.flashTask, APP_CPU_NUM);


}

void loop()
{

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

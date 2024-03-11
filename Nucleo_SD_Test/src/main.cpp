#include <Arduino.h>
#include <SPI.h>
#include <STM32FreeRTOS.h>
#include <SD.h>

#define ADC_SIZE 340
#define QUE_SIZE 40

struct Frame {

    uint32_t time;
    uint16_t adc[ADC_SIZE][3];
    uint8_t state;
};

/****************************************************/

QueueHandle_t quehandle;
ADC_HandleTypeDef hadc1;
Frame frame[QUE_SIZE];

void MX_ADC1_Init(void) {
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;  // Włączone skanowanie
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 3;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK) {
    Error_Handler();
  }

  // Konfiguracja kanału 0
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  // Konfiguracja kanału 1
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }

  // Konfiguracja kanału 2
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }
}

HardwareSerial Serial2(USART2);

/****************************************************/

void sd_init() {

    // INIT_SD:
    SPI.begin();
    if (SD.begin(1000000,PB0)) Serial2.println("SD Działa!");
    else Serial2.println("SD Nie Działa!");
}

void sd_append() {

    File dataFile;

    for (uint8_t j = 0; j < QUE_SIZE; j++) {

        // ZAPIS SD:
        dataFile = SD.open("datalog.bin", FILE_WRITE);
        dataFile.write((uint8_t*) &frame[j], sizeof(Frame));
        dataFile.close();
    }
}

/****************************************************/

void sd_task(void* args) {

    Serial2.begin(115200);
    Serial2.println("Working");

    Serial2.println("Task SD działa");

    // INIT_SD:
    SPI.begin();
    if (SD.begin(PB0)) Serial2.println("SD Działa!");
    File dataFile;

    while(1) {

        if (uxQueueMessagesWaiting(quehandle) > 20) {

            while (uxQueueMessagesWaiting(quehandle)) {

                Frame frame;
                xQueueReceive(quehandle, &frame, portMAX_DELAY);

                // ZAPIS SD:
                dataFile = SD.open("datalog.bin", FILE_WRITE);
                dataFile.write((uint8_t*) &frame, sizeof(Frame));
                dataFile.close();
            }
        }
        vTaskDelay(1);
    }
}


/****************************************************/

void setup() {

    MX_ADC1_Init();
    HAL_ADC_Start(&hadc1);

    Serial2.begin(115200);
    Serial2.println("Working");

    //quehandle = xQueueCreate(50, sizeof(Frame));
    //xTaskCreate(sd_task, "SD Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    //vTaskStartScheduler();

    sd_init();
}

void loop() {

    uint32_t lastMesTime;

    for (uint8_t j = 0; j < QUE_SIZE; j++) {

        frame[j].time = lastMesTime;

        for (uint32_t i = 0; i < ADC_SIZE; i++) {

            while (micros() - lastMesTime < 10);
            lastMesTime = micros();

            HAL_ADC_PollForConversion(&hadc1, 1);
            frame[j].adc[i][0] = HAL_ADC_GetValue(&hadc1);
            HAL_ADC_PollForConversion(&hadc1, 1);
            frame[j].adc[i][1] = HAL_ADC_GetValue(&hadc1);
            HAL_ADC_PollForConversion(&hadc1, 1);
            frame[j].adc[i][2] = HAL_ADC_GetValue(&hadc1);
        }
    }

    sd_append();
    //xQueueSend(quehandle, &frame, 2);
}

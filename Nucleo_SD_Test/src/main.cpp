#include <Arduino.h>
#include <SPI.h>
#include <STM32FreeRTOS.h>
#include <SD.h>

#define ADC_SIZE 340

struct Frame {

    uint32_t time;
    uint16_t adc[ADC_SIZE][3];
    uint8_t state;
};

/****************************************************/

QueueHandle_t quehandle;
ADC_HandleTypeDef hadc1;

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

/****************************************************/

void sd_task(void* args) {

    // INIT_SD:
    SPI.begin();
    SD.begin(PB0);
    File dataFile;

    while(1) {

        if (uxQueueMessagesWaiting(quehandle) > 20) {

            while (uxQueueMessagesWaiting(quehandle)) {

                Frame frame;
                xQueueReceive(quehandle, &frame, portMAX_DELAY);

                // ZAPIS SD:
                dataFile = SD.open("datalog.txt", FILE_WRITE);
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

    quehandle = xQueueCreate(50, sizeof(Frame));
    xTaskCreate(sd_task, "SD Task", 32768, NULL, 1, NULL);
}

void loop() {

    Frame frame;
    uint32_t lastMesTime;
    frame.time = lastMesTime;

    for (uint32_t i = 0; i < ADC_SIZE; i++) {

        while (micros() - lastMesTime < 10);
        lastMesTime = micros();

        HAL_ADC_PollForConversion(&hadc1, 1);
        frame.adc[i][0] = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 1);
        frame.adc[i][1] = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 1);
        frame.adc[i][2] = HAL_ADC_GetValue(&hadc1);
    }

    xQueueSend(quehandle, &frame, 2);
}

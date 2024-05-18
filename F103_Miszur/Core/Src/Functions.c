#include "Functions.h"

#include "usbd_cdc_if.h"

uint32_t dotOneMsTime;
volatile bool tickTime;
uint8_t rocketState;
uint16_t adctest[ADC_NO];

void doMeasurements(Frame* frame) {

	frame->time = dotOneMsTime;
	frame->state = rocketState;
	char measr[100];

	for (uint16_t i = 0; i < ADC_SIZE; i ++) {

		// Czekanie 10us:
		while (!tickTime);
		tickTime = 0;

		HAL_ADC_PollForConversion(&hadc1, 1);
		frame->adc[i][0] = HAL_ADC_GetValue(&hadc1) / 8;
		HAL_ADC_PollForConversion(&hadc1, 1);
		frame->adc[i][1] = HAL_ADC_GetValue(&hadc1) / 8;
		HAL_ADC_PollForConversion(&hadc1, 1);
		frame->adc[i][2] = HAL_ADC_GetValue(&hadc1) / 8;


		sprintf(measr, "%d;%d;%d;%d;3\n", (int)dotOneMsTime, (int)frame->adc[i][0], (int)frame->adc[i][1], (int)frame->adc[i][2]);
		CDC_Transmit_FS((uint8_t*) measr, strlen(measr));
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM1) {
		tickTime = 1;
		dotOneMsTime += 1;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // Check if UART1:
    if (huart->Instance == USART1)
    {
        // Next message IT request:
        HAL_UART_Receive_IT(&huart1, &rocketState, 1);
    }
}

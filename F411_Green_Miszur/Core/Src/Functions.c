#include "Functions.h"

uint32_t dotOneMsTime;
volatile bool tickTime;
uint8_t rocketState;
uint16_t adctest[ADC_NO];

void doMeasurements(Frame* frame) {

	frame->time = dotOneMsTime;
	frame->state = rocketState;

	for (uint16_t i = 0; i < ADC_SIZE; i ++) {

		// Czekanie 10us:
		while (!tickTime);
		tickTime = 0;

		frame->adc[i][0] = adctest[0];
		frame->adc[i][1] = adctest[1];
		frame->adc[i][2] = adctest[2];
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

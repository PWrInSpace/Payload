#include "Functions.h"

uint32_t us_time;
volatile bool tickTime;

void doMeasurements(Frame* frame) {

	frame->time = us_time;
	frame->state = 33;

	for (uint16_t i = 0; i < ADC_SIZE; i ++) {

		// Czekanie 10us:
		while (!tickTime);
		tickTime = 0;

		HAL_ADC_PollForConversion(&hadc1, 1);
		frame->adc[i][0] = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		frame->adc[i][1] = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		frame->adc[i][2] = HAL_ADC_GetValue(&hadc1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM2) {
		tickTime = 1;
		us_time += 10;
	}
}

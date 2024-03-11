#include "Functions.h"

uint32_t us_time;

void doMeasurements(Frame* frame) {

	frame->time = us_time;

	for (uint16_t i = 0; i < ADC_SIZE; i ++) {

		// TODO czekanie 10us.

		HAL_ADC_PollForConversion(&hadc1, 1);
		frame->adc[i][0] = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		frame->adc[i][1] = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		frame->adc[i][2] = HAL_ADC_GetValue(&hadc1);
	}
}

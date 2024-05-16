#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "adc.h"
#include <string.h>
#include <stdbool.h>

#define ADC_SIZE 680
#define QUE_SIZE 4
#define ADC_NO 3
#define SLOT_PAGE_NUMBER 26495
#define SLOT_A_OFFSET 128
#define SLOT_B_OFFSET 26752
#define CONFIGURATION_PAGE 64
#define PAGES_IN_BLOCK 64
#define BUF_SIZE 2048

typedef struct {

    uint32_t time;
    uint8_t adc[ADC_SIZE][ADC_NO];
    uint8_t state;

} Frame;

extern uint32_t dotOneMsTime;
extern uint8_t rocketState;
extern uint16_t adctest[ADC_NO];

void doMeasurements(Frame* frame);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_FUNCTIONS_H_ */

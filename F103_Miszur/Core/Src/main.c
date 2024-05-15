/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Functions.h"
#include "W25N01G.h"
#include "usbd_cdc_if.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

Frame frames[QUE_SIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  	// Init Everything in MCU:
	HAL_Delay(900);
	HAL_ADC_Start(&hadc1);
	HAL_TIM_Base_Init(&htim1);
	HAL_TIM_Base_MspInit(&htim1);
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_UART_Receive_IT(&huart1, &rocketState, 1);

	// Init Flash:
	uint8_t ret = 1;
	while(ret) {
	  ret = flashBegin();
	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, !ret);
	  HAL_Delay(1000);
	}

	HAL_Delay(3000);
	uint8_t flashBuf[BUF_SIZE];
	uint16_t slot_page_offset;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adctest, ADC_NO);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (rocketState < 6) {

		  for (uint8_t i = 0; i < 3; i++) {

			  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
			  HAL_Delay(250);
			  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
			  HAL_Delay(500);
		  }
		  HAL_GPIO_WritePin(USB_PULL_GPIO_Port, USB_PULL_Pin, 0);

		  // Here USB read:
		  while (!txRequest) HAL_Delay(1);

		  // SLOT A read:
		  slot_page_offset = SLOT_A_OFFSET;
		  char slot_info[] = "SLOT_A";
		  CDC_Transmit_FS((uint8_t*) slot_info, strlen(slot_info));

		  for (uint16_t j = 0; j < SLOT_PAGE_NUMBER; j++) {

			  pageDataRead(j + slot_page_offset);
			  read(0, flashBuf, BUF_SIZE);

			  _Bool empty = 1;
			  for (uint16_t i = 0; i < 2048; i++) {
				  if (flashBuf[i] != 255) empty = 0;
			  }
			  if (empty) break;
			  else CDC_Transmit_FS(flashBuf, sizeof(Frame));
		  }

		  // SLOT B read:
		  slot_page_offset = SLOT_B_OFFSET;
		  slot_info[5] = 'B';
		  HAL_Delay(10);
		  CDC_Transmit_FS((uint8_t*) slot_info, strlen(slot_info));
		  HAL_Delay(10);

		  for (uint16_t j = 0; j < SLOT_PAGE_NUMBER; j++) {

			  pageDataRead(j + slot_page_offset);
			  read(0, flashBuf, BUF_SIZE);

			  _Bool empty = 1;
			  for (uint16_t i = 0; i < 2048; i++) {
				  if (flashBuf[i] != 255) empty = 0;
			  }
			  if (empty) break;
			  else CDC_Transmit_FS(flashBuf, sizeof(Frame));
		  }
	  }

	  if (rocketState >= 6) {

		  // Flash slot change:
		  pageDataRead(CONFIGURATION_PAGE);
		  read(0, flashBuf, BUF_SIZE);

		  uint8_t isSlot_A = flashBuf[0];
		  flashBuf[0] = !isSlot_A;
		  blockErase(CONFIGURATION_PAGE);

		  if (isSlot_A) {
			  // If we are in slot A, save slot B for next reset:
			  slot_page_offset = SLOT_A_OFFSET;
		  }
		  else {
			  // If we are in slot B, save slot A for next reset:
			  slot_page_offset = SLOT_B_OFFSET;
		  }
		  loadProgData(0, flashBuf, BUF_SIZE);
		  ProgramExecute(CONFIGURATION_PAGE);

		  // Flash slot erase (only in state 6):
		  if (rocketState >= 6) {

			  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
			  for (uint16_t j = 0; j < SLOT_PAGE_NUMBER; j += PAGES_IN_BLOCK) {

				  blockErase(j + slot_page_offset);
			  }

			  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
			  HAL_Delay(1000);
		  }

		  // Save start:
		  for (uint16_t j = 0; j < SLOT_PAGE_NUMBER; j++) {

			  for (uint16_t i = 0; i < QUE_SIZE; i ++) {

				  doMeasurements(&frames[i]);
			  }

			  for (uint16_t i = 0; i < QUE_SIZE; i++) {

				  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
				  loadProgData(0, (uint8_t*) &frames[i], sizeof(Frame));
				  ProgramExecute(j + slot_page_offset);
				  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
			  }
		  }
	  }

	  while (1) { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0); }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

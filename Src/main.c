#include "main.h"
#include "crc.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "fmc.h"

#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"

  TS_StateTypeDef ts;

char _100_msec, _1_sec;
int i;
uint8_t button_press;
int count1;
uint8_t btn;
uint8_t oldbtn;
I2C_HandleTypeDef hi2c3;

void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

void BLUE_BUTTON (void){

	BSP_LCD_Clear(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
	BSP_LCD_DrawRect(80, 50, 80, 60);
	BSP_LCD_FillRect(80, 50, 80 , 60);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
	BSP_LCD_DrawRect(80, 210, 80, 60);
	BSP_LCD_FillRect(80, 210, 80, 60);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)" I'm BLUE", CENTER_MODE);

}
void MAGENTA_BUTTON (void){
	BSP_LCD_Clear(LCD_COLOR_LIGHTMAGENTA);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
	BSP_LCD_DrawRect(80, 50, 80, 60);
	BSP_LCD_FillRect(80, 50, 80 , 60);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
	BSP_LCD_DrawRect(80, 210, 80, 60);
	BSP_LCD_FillRect(80, 210, 80, 60);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_LIGHTMAGENTA);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"I'm MAGENTA", CENTER_MODE);
}

int main(void)
{
  //static char temp;
//	uint8_t cntr = 0;

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
//  MX_USART1_UART_Init();
//  MX_USB_HOST_Init();

  HAL_TIM_Base_Start_IT(&htim1);

  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(LCD_BACKGROUND_LAYER);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);//0x253617);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

  BSP_TS_Init(240, 320);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(80, 50, 80, 60);
  BSP_LCD_FillRect(80, 50, 80 , 60);

  BSP_LCD_SetTextColor(LCD_COLOR_MAGENTA);
  BSP_LCD_DrawRect(80, 210, 80, 60);
  BSP_LCD_FillRect(80, 210, 80, 60);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 145, (uint8_t *)"CAN YOU CLICK", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)" MY RECTANGLE?", CENTER_MODE);
//  for( uint8_t i=0 ; i<30 ; i++ )
//	  BSP_LCD_DrawLine( 60 , 120+i , 180 , 120+i );
  button_press=0;

  detect = 0;
  while (1)
  {
	if(_100_msec == 1){
	  _100_msec = 0;
		BSP_TS_GetState(&ts);
		if(detect == 1){
			detect = 0;
			if((80<=ts.X && ts.X<=160) && (50<=ts.Y && 110>=ts.Y)){
				oldbtn = button_press;
				button_press = 1;
			}
			else if((80<=ts.X && ts.X<=160) && (210<=ts.Y && 270>=ts.Y)){
				oldbtn = button_press;
				button_press = 2;

			}
			else{
				oldbtn = btn;
				btn = 3;

			}
			if(oldbtn != 1 && button_press == 1){
				BLUE_BUTTON();

			}
			if(oldbtn != 2 && button_press == 2){
				MAGENTA_BUTTON();

			}
		}
	}
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 50;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
if(htim->Instance == TIM1){
	i++;
	if((i % 10) == 0){
		_1_sec = 1;
	}
	_100_msec = 1;

}
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

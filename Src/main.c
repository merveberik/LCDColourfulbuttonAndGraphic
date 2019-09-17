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
#include "math.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"

  TS_StateTypeDef ts;

char _100_msec, _1_sec;
int i;
uint8_t button_press;
int count1, m;
uint8_t y;
uint8_t btn;
//uint8_t y;
uint8_t oldbtn;


I2C_HandleTypeDef hi2c3;
char returning;
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

void BACK_RECT (void){

	BSP_LCD_SetTextColor(LCD_COLOR_MAGENTA);
	BSP_LCD_DrawRect(30, 250, 80, 40);
	BSP_LCD_FillRect(30, 250, 80, 40);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(55, 265, (uint8_t *)"BACK", LEFT_MODE);
}

void GRAPHICS (void){

	BSP_LCD_Clear(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
	BSP_LCD_DrawRect(30, 90, 80, 50);
	BSP_LCD_FillRect(30, 90, 80, 50);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(32, 110, (uint8_t *)"Sinus Graph", LEFT_MODE);
	BACK_RECT();

}

void ABOUT (void){

	BSP_LCD_Clear(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAtLine(1,(uint8_t*)" BILKON LTD.STI");
	BSP_LCD_DisplayStringAtLine(3,(uint8_t*)" MERVE BERIK");
	BACK_RECT();

}

void LCD_INIT(void){
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_DrawRect(30, 30, 80, 50);
	BSP_LCD_FillRect(30, 30, 80 , 50);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(42, 50, (uint8_t *)"Graphics", LEFT_MODE);

	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_DrawRect(120, 250, 80, 40);
	BSP_LCD_FillRect(120, 250, 80, 40);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(140, 262, (uint8_t *)"About", LEFT_MODE);
}
//void loop(int val){
//	int i;
//	for(i = 0; i < val; i++){
//	}
//}
void SIN_GRAPH(void){
static int i;
static int val;
i = 0;
val = 1000;
	BSP_LCD_Clear(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawHLine(40, 220, 160);
	BSP_LCD_DrawVLine(40, 60, 160);
	BSP_LCD_DrawHLine(40, 220, 160);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(100, 225, (uint8_t *)"x-axis", LEFT_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(23, 45, (uint8_t *)"y-axis", LEFT_MODE);
	BACK_RECT();
	for(uint8_t x=0, y=0; x<150; x++, y++){
		for(i = 0; i < val; i++){
			x=y;
			BSP_LCD_DrawPixel(40+x, 220-y, LCD_COLOR_BLACK);
//			loop(15000);
		}
	}
//	for(uint8_t x=0; x<20; x++){
//		y=sin(x);
//		BSP_LCD_DrawPixel(x, y, LCD_COLOR_BLACK);
//	}
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
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);//0x253617);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_TS_Init(240, 320);

  LCD_INIT();

//  for( uint8_t i=0 ; i<30 ; i++ )
//	  BSP_LCD_DrawLine( 60 , 120+i , 180 , 120+i );
  button_press=0;
m = 0;
  detect = 0;
  while (1)
  {
	if(_100_msec == 1){
	  _100_msec = 0;
		BSP_TS_GetState(&ts);

		if(detect == 1){
			detect = 0;

			if((30<=ts.X && ts.X<=110) && (30<=ts.Y && 80>=ts.Y)){
				oldbtn = button_press;
				button_press = 1;
			}
			else if((120<=ts.X && ts.X<=200) && (250<=ts.Y && 290>=ts.Y)){
				oldbtn = button_press;
				button_press = 2;

			}
			else if((30<=ts.X && ts.X<=110) && (250<=ts.Y && 290>=ts.Y)){
					oldbtn = button_press;
					button_press = 3;
			}
			else if((30<=ts.X && ts.X<=110) && (90<=ts.Y && 140>=ts.Y)){
					oldbtn = button_press;
					button_press = 4;
			}

			if(oldbtn != 1 && button_press == 1){
				GRAPHICS();
			}
			if(oldbtn != 2 && button_press == 2){
				ABOUT();
			}
			if(oldbtn != 3 && button_press == 3){
				LCD_INIT();
			}
			if(oldbtn != 4 && button_press == 4){
				SIN_GRAPH();

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
		m++;
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

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sh1122.h"
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  // Init display
  Display_Init();
  //Display_SetOrienation(OLED_DISP_ROTATE180);

  uint16_t X_start = OLED_WIDTH - 90;

  // Names
  Frame_printf(X_start / 2, OLED_HEIGHT / 4 - 2, FONTID_10X16F, Display_Color.Gray_15, CENTER, CENTER, "Компьютер");
  Frame_printf(X_start / 2, 3 * OLED_HEIGHT / 4 + 2, FONTID_10X16F, Display_Color.Gray_15, CENTER, CENTER, "Усилитель\r\n\"Prometheus\"");

  //Frame_DrawRectangle(X_start + 20, 0, OLED_WIDTH - 1, 63, Display_Color.Gray_01);

  // Arrow
  Frame_DrawLine(X_start, OLED_HEIGHT / 4 - 2, X_start + 15, OLED_HEIGHT / 4 - 2, Display_Color.Gray_15);
  Frame_DrawLine(X_start, OLED_HEIGHT / 4 - 3, X_start + 15, OLED_HEIGHT / 4 - 3, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 9, OLED_HEIGHT / 4 - 3 - 6, X_start + 15, OLED_HEIGHT / 4 - 3, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 10, OLED_HEIGHT / 4 - 3 - 6, X_start + 16, OLED_HEIGHT / 4 - 3, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 9, OLED_HEIGHT / 4 - 2 + 6, X_start + 15, OLED_HEIGHT / 4 - 2, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 10, OLED_HEIGHT / 4 - 2 + 6, X_start + 16, OLED_HEIGHT / 4 - 2, Display_Color.Gray_15);
  // Arrow
  Frame_DrawLine(X_start, 3 * OLED_HEIGHT / 4 + 2, X_start + 15, 3 * OLED_HEIGHT / 4 + 2, Display_Color.Gray_15);
  Frame_DrawLine(X_start, 3 * OLED_HEIGHT / 4 + 3, X_start + 15, 3 * OLED_HEIGHT / 4 + 3, Display_Color.Gray_15);
  Frame_DrawLine(X_start, 3 * OLED_HEIGHT / 4 + 2, X_start + 6, 3 * OLED_HEIGHT / 4 + 2 - 6, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 1, 3 * OLED_HEIGHT / 4 + 2, X_start + 7, 3 * OLED_HEIGHT / 4 + 2 - 6, Display_Color.Gray_15);
  Frame_DrawLine(X_start, 3 * OLED_HEIGHT / 4 + 3, X_start + 6, 3 * OLED_HEIGHT / 4 + 3 + 6, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 1, 3 * OLED_HEIGHT / 4 + 3, X_start + 7, 3 * OLED_HEIGHT / 4 + 3 + 6, Display_Color.Gray_15);

  // Numbers
  //Frame_DrawRectangle(X_start + 22, 0, X_start + 53, 27, Display_Color.Gray_15);
  Frame_printf(X_start + 38, OLED_HEIGHT / 4 - 2, FONTID_24F, Display_Color.Gray_15, CENTER, CENTER, "20");

  //Frame_DrawRectangle(X_start + 22, 3 * OLED_HEIGHT / 4 - 12, X_start + 53, 3 * OLED_HEIGHT / 4 + 15, Display_Color.Gray_15);
  Frame_printf(X_start + 38, 3 * OLED_HEIGHT / 4 + 2, FONTID_24F, Display_Color.Gray_15, CENTER, CENTER, "12");

  // Locked
  Frame_printf(X_start + 80, OLED_HEIGHT / 2 + 1, FONTID_10X16F, Display_Color.Gray_15, CENTER, CENTER, "%c", 0x1F); // Lock symbol
  
  // Unmute
  Frame_DrawLine(X_start + 54, OLED_HEIGHT / 4 - 2, X_start + 61, OLED_HEIGHT / 4 - 2, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 54, 3 * OLED_HEIGHT / 4 + 2, X_start + 61, 3 * OLED_HEIGHT / 4 + 2, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 62, OLED_HEIGHT / 4 - 1, X_start + 62, 3 * OLED_HEIGHT / 4 + 1, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 62 - 6, OLED_HEIGHT / 2 - 3, X_start + 62, OLED_HEIGHT / 2 + 3, Display_Color.Gray_15);
  Frame_DrawLine(X_start + 62 + 6, OLED_HEIGHT / 2 - 3, X_start + 62, OLED_HEIGHT / 2 + 3, Display_Color.Gray_15);

  
  Display_SendFrame();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

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

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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

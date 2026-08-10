/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

void TIM3_IRQHandler(void)
{
    // clear the interrupt flag
	TIM3->SR &= ~TIM_SR_UIF;
    // toggle the LED
	GPIOA->ODR ^= GPIO_ODR_OD5;

}

void uart_send_char(char c)
{
    while (!(USART2->ISR & USART_ISR_TXE_TXFNF));
    USART2->TDR = c;
}


void uart_send_string(const char *str)
{
    while (*str)
    {
        uart_send_char(*str++);
    }
}
#include <errno.h>
#include <sys/unistd.h>

int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        uart_send_char(ptr[i]);
    }
    return len;
}
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
  /* USER CODE BEGIN 2 */


  /* Infinite loop */
  /*RCC->APBRSTR1 |= RCC_APBRSTR1_USART2RST;
  RCC->APBRSTR1 &= ~RCC_APBRSTR1_USART2RST;*/
  	  /* USER CODE BEGIN WHILE */
  	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  	// First clear both bits (in case they default to something else)
  	GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
  	// Then set 01 for output
  	GPIOA->MODER |= GPIO_MODER_MODE5_0;
  	GPIOA->ODR &= ~GPIO_ODR_OD5_Msk;

  	// TIMER
  	RCC->APBENR1 |= RCC_APBENR1_TIM3EN;
  	TIM3->PSC = 47999;
  	TIM3->ARR = 999;
  	TIM3->CR1 |= TIM_CR1_CEN;
  	TIM3->DIER = TIM_DIER_UIE;
  	NVIC_EnableIRQ(TIM3_IRQn);

  	// UART
  	// PA already enabled
  	GPIOA->MODER &= ~GPIO_MODER_MODE2_Msk;
  	GPIOA->MODER |= GPIO_MODER_MODE2_1;
  	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_Msk; // [0] for a pin <8, sel is for the AF #
  	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;

  	GPIOA->MODER &= ~GPIO_MODER_MODE3_Msk;
  	GPIOA->MODER |= GPIO_MODER_MODE3_1;
  	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_Msk;
  	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;

  	// clock for UART (order matters)
  	RCC->APBENR1 |= RCC_APBENR1_USART2EN;
  	USART2->BRR = 417;
  	USART2->CR1 |= USART_CR1_TE;
  	USART2->CR1 |= USART_CR1_UE;
  	USART2->TDR;

  	/*	 PWM setup  - not in blink	 */

  	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
  	GPIOB->MODER &= ~GPIO_MODER_MODE0_Msk;
  	GPIOB->MODER |= GPIO_MODER_MODE0_1;
  	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL0_Msk;
  	  	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL0_0;

//  	  	GPIOB->MODER &= ~GPIO_MODER_MODE3_Msk;
//  	  	GPIOB->MODER |= GPIO_MODER_MODE3_1;
//  	  	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL0_Msk;
//  	  	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL0_0;

  	  	// clock for UART (order matters)
  	  	RCC->APBENR1 |= RCC_APBENR1_USART2EN;
  	  	USART2->BRR = 417;
  	  	USART2->CR1 |= USART_CR1_TE;
  	  	USART2->CR1 |= USART_CR1_UE;
  	  	USART2->TDR;

  while (1)
  {
	  //uart_send_string("Hello World\r\n");
	  printf("Hello World %d\r\n", 42);
	  //uart_send_char('U');
	      for (volatile uint32_t i = 0; i < 500000; i++);
//	  GPIOA->ODR |= GPIO_ODR_OD5;
//    /* USER CODE END WHILE */
//	  for (volatile uint32_t i = 0; i < 100000; i++);
//	  GPIOA->ODR &= ~GPIO_ODR_OD5_Msk;
//	  for (volatile uint32_t i = 0; i < 100000; i++);
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

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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

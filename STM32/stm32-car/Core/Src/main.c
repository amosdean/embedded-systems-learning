/*
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  *
  ******************************************************************************
  */

#include "main.h"
#include "motor.h"
#include "scheduler.h"
#include "timer.h"
#include "uart.h"

void SystemClock_Config(void);
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  uart_init();
    const Maneuver sequence[] = {
    {FORWARD,  FORWARD,  200, 200, 200},  // straight, 2 sec
    // {BACKWARD,  BACKWARD, 200, 200, 50},   // turn left, 0.5 sec
    // {FORWARD,  FORWARD,  200, 200, 200},  // straight, 2 sec
    // {FORWARD,  BACKWARD, 150, 150, 50},   // turn left, 0.5 sec
    // {FORWARD,  FORWARD,  200, 200, 200},  // straight again
    // {RELEASE,  RELEASE,  0,   0,   100},  // stop
    };
    Scheduler scheduler = {
        .sequence = sequence,
        .length = sizeof(sequence) / sizeof(sequence[0]),
        .current_maneuver = 0,
        .elapsedTime = 0
    };

    initMotor(); // motors must be initialized before schedule or timer
    timer_init();
    startScheduler(scheduler); // start first maneuver or will wait until first tick

    while (1)
    {
      if (counter >= 1) {
          tickScheduler(&scheduler);
          // uart_send_string(buf);
          // uart_send_string("Ticking scheduler\r\n");
          counter = 0;
      }
    }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  // C031 boots at 48MHz HSI by default
  // no additional configuration needed
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
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
  
}
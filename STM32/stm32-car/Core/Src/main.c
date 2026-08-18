/*
/**
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


void SystemClock_Config(void);
int main(void)
{
  
  SystemClock_Config();

    const Maneuver sequence[] = {
    {FORWARD,  FORWARD,  200, 200, 200},  // straight, 2 sec
    {BACKWARD,  BACKWARD, 200, 200, 50},   // turn left, 0.5 sec
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
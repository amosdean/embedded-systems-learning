#include "timer.h"
#include "stm32c0xx.h"
volatile uint8_t counter = 0;

void SysTick_Handler(void) {
    counter++;
}

void timer_init() {
    SysTick_Config(48000); // 48MHz / 48000 = 1ms tick
}
#include "uart.h"

void uart_init(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    // PA2 TX, PA3 RX - AF1
    GPIOA->MODER &= ~GPIO_MODER_MODE2_Msk;
    GPIOA->MODER |= GPIO_MODER_MODE2_1;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_Msk;
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;
    GPIOA->MODER &= ~GPIO_MODER_MODE3_Msk;
    GPIOA->MODER |= GPIO_MODER_MODE3_1;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_Msk;
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;

    RCC->APBENR1 |= RCC_APBENR1_USART2EN;
    USART2->BRR = 417;
    USART2->CR1 |= USART_CR1_TE;
    USART2->CR1 |= USART_CR1_UE;
}

void uart_send_char(char c)
{
    while (!(USART2->ISR & USART_ISR_TXE_TXFNF));
    USART2->TDR = c;
}

void uart_send_string(const char *str)
{
    while (*str)
        uart_send_char(*str++);
}

int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
        uart_send_char(ptr[i]);
    return len;
}
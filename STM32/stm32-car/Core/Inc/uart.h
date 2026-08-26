#ifndef UART_H
#define UART_H

#include "stm32c0xx.h"
#include <stdio.h>

void uart_init(void);
void uart_send_char(char c);
void uart_send_string(const char *str);

#endif
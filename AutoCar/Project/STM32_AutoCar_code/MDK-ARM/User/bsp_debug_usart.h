#ifndef __DEBUG_USART_H
#define __DEBUG_USART_H

#include "stm32f1xx.h"
#include <stdio.h>



void uart_FlushRxBuffer(void);
void Usart_SendByte(uint8_t str);
void Usart_SendString(uint8_t *str);
void DEBUG_USART_Config(void);
//int fputc(int ch, FILE *f);
extern UART_HandleTypeDef UartHandle;
#endif /* __USART1_H */

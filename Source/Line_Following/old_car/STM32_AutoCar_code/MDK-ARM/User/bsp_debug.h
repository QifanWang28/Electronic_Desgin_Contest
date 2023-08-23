#ifndef __BSP_DEBUG_H
#define __BSP_DEBUG_H

#include "stm32f1xx.h"
#include <stdio.h>

//串口接收缓冲数组大小
#define UART_RX_BUFFER_SIZE 256 

extern unsigned char UART_RxBuffer[UART_RX_BUFFER_SIZE];
extern uint8_t receive_cmd;
extern unsigned char UART_RxPtr;


void uart_FlushRxBuffer(void);
void Usart_SendByte(uint8_t str);
void Usart_SendString(uint8_t *str);
//int fputc(int ch, FILE *f);



#endif



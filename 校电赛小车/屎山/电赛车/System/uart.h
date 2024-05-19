#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"                  // Device header

void USARTx_Init(void);//串口1初始化并启动
uint8_t Send(void);
uint8_t colorsearch(void);
uint8_t x_position(void);

#endif

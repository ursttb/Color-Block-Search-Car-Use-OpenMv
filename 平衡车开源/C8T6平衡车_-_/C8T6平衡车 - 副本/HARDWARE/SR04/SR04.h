#ifndef __HC_SR04_H
#define __HC_SR04_H
#include "sys.h"


void SR04_Init(void);
void TIM1_CC_IRQHandler(void);
//获取计数器值
u32 GetEchoTimer(void);

//通过定时器3计数器值推算距离
u16 GetLength(void);

float Read_TIM1Distane(void);

#endif



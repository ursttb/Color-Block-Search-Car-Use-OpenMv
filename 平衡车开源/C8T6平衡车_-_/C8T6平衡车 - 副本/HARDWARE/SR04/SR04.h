#ifndef __HC_SR04_H
#define __HC_SR04_H
#include "sys.h"


void SR04_Init(void);
void TIM1_CC_IRQHandler(void);
//��ȡ������ֵ
u32 GetEchoTimer(void);

//ͨ����ʱ��3������ֵ�������
u16 GetLength(void);

float Read_TIM1Distane(void);

#endif



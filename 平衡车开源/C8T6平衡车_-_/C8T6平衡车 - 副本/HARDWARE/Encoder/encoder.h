#ifndef __ENCODER_H
#define __ENCODER_H

//PA6,PA7��������Ӧ��IO�ڶ�ʱ��
void encoder_TIM3_Init(void);

//PB6,PB7��������Ӧ��IO�ڶ�ʱ��
void encoder_TIM4_Init(void);

int Read_Speed(int TIMx);


void TIM4_IRQHandler(void);



#endif



#ifndef __CONTROL_H
#define __CONTROL_H


//��ʱ��ȡ�õĶ�ʱ��
#define PID_TIM TIM2
#define NVIC_Channel_PID_TIM TIM2_IRQn
#define RCC_PID_TIM RCC_APB1Periph_TIM2
#define PID_TIM_IRQHandler TIM2_IRQHandler

//PID�жϴ���
void PID_Init(void);

int Displacement(float Med, float SR04_Num);
int Velocity(float Med, int16_t Encode_A, int16_t Encode_B, float Acc);
int Final_PWM_Out(float Displacement_PWM_Out, int16_t Encode_A, int16_t Encode_B, float Acc);

#endif

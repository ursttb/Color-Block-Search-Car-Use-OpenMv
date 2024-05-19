#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f10x.h"                  // Device header


//��ʱ��ȡ�õĶ�ʱ��
#define PID_TIM TIM2
#define NVIC_Channel_PID_TIM TIM2_IRQn
#define RCC_PID_TIM RCC_APB1Periph_TIM2
#define PID_TIM_IRQHandler TIM2_IRQHandler

extern uint8_t Mode;//С��ģʽ

//������������
//�ж����޺�ɫ����
extern uint8_t Redflag;
//��ɫ����x����
extern uint8_t Red_x;
//�Ƕ�
extern int16_t angle; extern int16_t turn_dir;
//���ٶ�
extern float AX, AY, AZ; extern int16_t GX, GY, GZ;
//�������ٶ�
extern int16_t Encoder_left, Encoder_right;
//λ��
extern float Distance;
//�����ֵ
extern int Out_Displacement,Out_Velocity;//λ�ƻ����ֵ���ٶȻ����ֵ
extern int PWM_Out;//���������ֵ

//��е��ֵ
extern float Med_Displacement;

//PID�жϴ���
void PID_Init(void);

int Displacement(float Med, float SR04_Num);
int Velocity(float Med, int16_t Encode_A, int16_t Encode_B, float Acc);
int Turnment(uint8_t Med, uint8_t x_position);
int Final_PWM_Out(float Displacement_PWM_Out, int16_t Encode_A, int16_t Encode_B, float Acc);
void Set_Med_Displacement(float* Med_Displacement);

#endif

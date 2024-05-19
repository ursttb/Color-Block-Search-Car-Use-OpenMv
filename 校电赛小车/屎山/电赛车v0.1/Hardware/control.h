#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f10x.h"                  // Device header


//定时读取用的定时器
#define PID_TIM TIM2
#define NVIC_Channel_PID_TIM TIM2_IRQn
#define RCC_PID_TIM RCC_APB1Periph_TIM2
#define PID_TIM_IRQHandler TIM2_IRQHandler

extern uint8_t Mode;//小车模式

//传感器各数据
//判断有无红色方块
extern uint8_t Redflag;
//红色方块x坐标
extern uint8_t Red_x;
//角度
extern int16_t angle; extern int16_t turn_dir;
//加速度
extern float AX, AY, AZ; extern int16_t GX, GY, GZ;
//编码器速度
extern int16_t Encoder_left, Encoder_right;
//位移
extern float Distance;
//输出环值
extern int Out_Displacement,Out_Velocity;//位移环输出值和速度环输出值
extern int PWM_Out;//最终输出环值

//机械中值
extern float Med_Displacement;

//PID中断处理
void PID_Init(void);

int Displacement(float Med, float SR04_Num);
int Velocity(float Med, int16_t Encode_A, int16_t Encode_B, float Acc);
int Turnment(uint8_t Med, uint8_t x_position);
int Final_PWM_Out(float Displacement_PWM_Out, int16_t Encode_A, int16_t Encode_B, float Acc);
void Set_Med_Displacement(float* Med_Displacement);

#endif

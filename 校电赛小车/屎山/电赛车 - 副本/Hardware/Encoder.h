#ifndef __ENCODER_H
#define __ENCODER_H

//编码器用得定时器
#define Encoder_Tim TIM3
#define RCC_Encoder_Tim RCC_APB1Periph_TIM3
#define Encoder_Tim_Channel_A TIM_Channel_3
#define Encoder_Tim_Channel_B TIM_Channel_4

//编码器AB相的GPIO口
#define Encoder_GPIO GPIOA
#define RCC_Encoder_GPIO RCC_APB2Periph_GPIOA
#define RCC_Encoder_GPIO_Pin GPIO_Pin_6 | GPIO_Pin_7

//定时读取用的定时器
#define Read_Encoder_Tim TIM2
#define NVIC_Channel_Read_Encoder_Tim TIM2_IRQn
#define RCC_Read_Encoder_Tim RCC_APB1Periph_TIM2
#define Read_Encoder_Tim_IRQHandler TIM2_IRQHandler

extern int16_t speed;
void Encoder_Init(void);
int Get_Encoder(void);

#endif

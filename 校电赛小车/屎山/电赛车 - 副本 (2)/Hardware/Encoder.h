#ifndef __ENCODER_H
#define __ENCODER_H


//编码器用得定时器(左轮)
#define Encoder_Tim TIM3
#define RCC_Encoder_Tim RCC_APB1Periph_TIM3
#define Encoder_Tim_Channel_A TIM_Channel_1
#define Encoder_Tim_Channel_B TIM_Channel_2

//编码器AB相的GPIO口(左轮)
#define Encoder_GPIO GPIOA
#define RCC_Encoder_GPIO RCC_APB2Periph_GPIOA
#define RCC_Encoder_GPIO_Pin GPIO_Pin_6 | GPIO_Pin_7

//编码器用得定时器(右轮)
#define Encoder_TimR TIM4
#define RCC_Encoder_TimR RCC_APB1Periph_TIM4
#define Encoder_TimR_Channel_A TIM_Channel_1
#define Encoder_TimR_Channel_B TIM_Channel_2

//编码器AB相的GPIO口(右轮)
#define Encoder_GPIOR GPIOB
#define RCC_Encoder_GPIOR RCC_APB2Periph_GPIOB
#define RCC_Encoder_GPIO_PinR GPIO_Pin_6 | GPIO_Pin_7


void Encoder_Init(void);
//外部引用读取时括号内的数值
#define LEFT_ENCODER 3
#define RIGHT_ENCODER 4
int16_t Get_Encoder(int x);

#endif

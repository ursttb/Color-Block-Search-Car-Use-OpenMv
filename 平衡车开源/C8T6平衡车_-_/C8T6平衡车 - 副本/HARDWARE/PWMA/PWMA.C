#include "PWMA.H"
#include "sys.h"
#include "usart.h"



void PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	
	//时钟使能：定时器5时钟，GPIO和复用口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	

	//PA2，PWMA的GPIO口初始化
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //复用推挽输出  TIM2_CH3
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;					
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PA3,PWMB的GPIO口初始化
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //复用推挽输出  TIM2_CH4
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;					
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	//初始化TIM5
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=arr-1;    	//设置重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;   //设置预分频数
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	

	
	//初始化PWM模式
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC3Init(TIM2,&TIM_OCInitStruct);

	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC4Init(TIM2,&TIM_OCInitStruct);
	
	
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);  //预重装值，开启输出PWM
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable); 	//预重装值，开启输出PWM
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);//启动自动重装
	TIM_Cmd(TIM2,ENABLE);
	
}




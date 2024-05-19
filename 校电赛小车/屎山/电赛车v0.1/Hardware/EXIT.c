#include "stm32f10x.h"                  // Device header
#include "ENCODER.h"
#include "CONTROL.h"

//void MPU6050_EXTI_Init(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
//	
//	EXTI_InitTypeDef EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_Init(&EXTI_InitStructure);
//	
//}

void EXIT_Init()
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruture;
	NVIC_InitStruture.NVIC_IRQChannel = NVIC_Channel_PID_TIM;//定时中断PID处理
	NVIC_InitStruture.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruture.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruture.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruture);
	
}

#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void PWM_Init()
{
	RCC_APB2PeriphClockCmd(RCC_PWM_TIM, ENABLE);//若修改定时器记得修改
	RCC_APB2PeriphClockCmd(RCC_PWM_TIM_GPIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = PWM_A_TIM_GPIO_Pin | PWM_B_TIM_GPIO_Pin | PWM_C_TIM_GPIO_Pin | PWM_D_TIM_GPIO_Pin;//四路PWM波控制转速
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWM_TIM_GPIO, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(PWM_TIM);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//正转有效，反转时候应该将极性反转；函数:TIM_OCxPolarityConfig();
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	PWM_A_TIM_OC_Init(PWM_TIM, &TIM_OCInitStructure);
	PWM_B_TIM_OC_Init(PWM_TIM, &TIM_OCInitStructure);
	PWM_C_TIM_OC_Init(PWM_TIM, &TIM_OCInitStructure);
	PWM_D_TIM_OC_Init(PWM_TIM, &TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(PWM_TIM,ENABLE);	
	
	TIM_ARRPreloadConfig(PWM_TIM,ENABLE);
	OCA_PreloadConfig(PWM_TIM,ENABLE);
	OCB_PreloadConfig(PWM_TIM,ENABLE);
	OCC_PreloadConfig(PWM_TIM,ENABLE);
	OCD_PreloadConfig(PWM_TIM,ENABLE);
	
	TIM_Cmd(PWM_TIM,ENABLE);
	

}

//void PWM_A_SetCompare1(uint16_t Psc)
//{
//	OCA_SetCompare(PWM_TIM, Psc);
//}

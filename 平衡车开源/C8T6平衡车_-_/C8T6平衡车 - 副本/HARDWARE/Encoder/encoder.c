	#include "encoder.h"
#include "sys.h"


void encoder_TIM3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;

	
	//时钟使能：定时器3时钟，GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	

	//PA6,PA7.GPIO口初始化
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //浮空输入	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;		//PA6 TIM3_CH1  PA7 TIM3_CH2			
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	
	
	//初始化TIM3
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct); 
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=65535;    	//设置重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;   //设置预分频数
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//配置编码器模式
	
	//输入捕获配置
	TIM_ICStructInit(&TIM_ICInitStruct); 
	TIM_ICInitStruct.TIM_ICFilter=10;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);

//	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM3,0);
	
	TIM_Cmd(TIM3,ENABLE);
}



void encoder_TIM4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;

	
	//时钟使能：定时器4时钟，GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	

	//PB6,PB7.GPIO口初始化
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //浮空输入	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;		//PB6 TIM4_CH1  PB7 TIM4_CH2			
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	
	
	//初始化TIM4
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct); 
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=65535;    	//设置重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;   //设置预分频数
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//配置编码器模式
	
	//输入捕获配置
	TIM_ICStructInit(&TIM_ICInitStruct); 
	TIM_ICInitStruct.TIM_ICFilter=10;
	TIM_ICInit(TIM4,&TIM_ICInitStruct);

//	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM4,0);

	TIM_Cmd(TIM4,ENABLE);
}


int Read_Speed(int TIMx)
{
		int value_1;
	switch(TIMx)
	{
		case 3:value_1=(short)TIM_GetCounter(TIM3);	TIM_SetCounter(TIM3,0);break;
		case 4:value_1=(short)TIM_GetCounter(TIM4);	TIM_SetCounter(TIM4,0);break;
		default:value_1=0;
	}
	return value_1;
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=0)
	{	
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=0)
	{	
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}




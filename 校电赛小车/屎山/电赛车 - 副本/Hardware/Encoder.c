#include "stm32f10x.h"                  // Device header
#include "ENCODER.H"

int16_t speed = 0;

void Read_Encoder_Tim_Init()
{
	RCC_APB1PeriphClockCmd(RCC_Read_Encoder_Tim, ENABLE);
	
	TIM_InternalClockConfig(Read_Encoder_Tim);
	
	TIM_TimeBaseInitTypeDef TimeBaseInitStruture;
	TimeBaseInitStruture.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBaseInitStruture.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStruture.TIM_Period = 1000 - 1;
	TimeBaseInitStruture.TIM_Prescaler = 7200 - 1;
	TimeBaseInitStruture.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(Read_Encoder_Tim, &TimeBaseInitStruture);
	
	TIM_ClearFlag(Read_Encoder_Tim, TIM_FLAG_Update);
	TIM_ITConfig(Read_Encoder_Tim, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruture;
	NVIC_InitStruture.NVIC_IRQChannel = NVIC_Channel_Read_Encoder_Tim;
	NVIC_InitStruture.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruture.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruture.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruture);
	
	TIM_Cmd(Read_Encoder_Tim,ENABLE);
	
}

void Read_Encoder_Tim_IRQHandler()
{
	if(TIM_GetITStatus(Read_Encoder_Tim, TIM_IT_Update) == SET)
	{
		speed = Get_Encoder();
		TIM_ClearITPendingBit(Read_Encoder_Tim, TIM_IT_Update);
	}
}

void Encoder_Init()
{
	Read_Encoder_Tim_Init();
	
	RCC_APB1PeriphClockCmd(RCC_Encoder_Tim, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_Encoder_GPIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruture;
	GPIO_InitStruture.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruture.GPIO_Pin = RCC_Encoder_GPIO_Pin;
	GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Encoder_GPIO, &GPIO_InitStruture);
	
	TIM_TimeBaseInitTypeDef TimeBaseInitStruture;
	TimeBaseInitStruture.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBaseInitStruture.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStruture.TIM_Period = 65536 - 1;
	TimeBaseInitStruture.TIM_Prescaler = 0;
	TimeBaseInitStruture.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(Encoder_Tim, &TimeBaseInitStruture);
	
	TIM_ICInitTypeDef ICInitStruture;
	TIM_ICStructInit(&ICInitStruture);
	ICInitStruture.TIM_Channel = Encoder_Tim_Channel_A;
	ICInitStruture.TIM_ICFilter = 0xF;
	TIM_ICInit(Encoder_Tim, &ICInitStruture);
	ICInitStruture.TIM_Channel = Encoder_Tim_Channel_B;
	ICInitStruture.TIM_ICFilter = 0xF;
	TIM_ICInit(Encoder_Tim, &ICInitStruture);
	
	TIM_EncoderInterfaceConfig(Encoder_Tim, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(Encoder_Tim, ENABLE);
}

int Get_Encoder()
{
	int temp;
	temp = TIM_GetCounter(Encoder_Tim);
	TIM_SetCounter(Encoder_Tim, 0);
	return temp;
}

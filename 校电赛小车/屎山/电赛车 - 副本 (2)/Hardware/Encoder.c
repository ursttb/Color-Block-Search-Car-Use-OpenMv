#include "stm32f10x.h"                  // Device header
#include "ENCODER.H"

void Encoder_Init()
{
	
	RCC_APB1PeriphClockCmd(RCC_Encoder_Tim, ENABLE);//×óÂÖ±àÂëÆ÷¶¨Ê±Æ÷¶Ë¿Ú
	RCC_APB2PeriphClockCmd(RCC_Encoder_GPIO, ENABLE);//×óÂÖ±àÂëÆ÷¶Ë¿Ú
	
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
	TIM_ICInit(Encoder_Tim, &ICInitStruture);//×óÂÖ±àÂëÆ÷ÉèÖÃ
	
	TIM_EncoderInterfaceConfig(Encoder_Tim, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(Encoder_Tim, ENABLE);
	
	//ÓÒÂÖÉèÖÃ
	RCC_APB1PeriphClockCmd(RCC_Encoder_TimR, ENABLE);//ÓÒÂÖ±àÂëÆ÷¶¨Ê±Æ÷¶Ë¿Ú
	RCC_APB2PeriphClockCmd(RCC_Encoder_GPIOR, ENABLE);//ÓÒÂÖ±àÂëÆ÷¶Ë¿Ú
	
	GPIO_InitStruture.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruture.GPIO_Pin = RCC_Encoder_GPIO_PinR;
	GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Encoder_GPIOR, &GPIO_InitStruture);
	
	TimeBaseInitStruture.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBaseInitStruture.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStruture.TIM_Period = 65536 - 1;
	TimeBaseInitStruture.TIM_Prescaler = 0;
	TimeBaseInitStruture.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(Encoder_TimR, &TimeBaseInitStruture);
	
	TIM_ICStructInit(&ICInitStruture);
	ICInitStruture.TIM_Channel = Encoder_TimR_Channel_A;
	ICInitStruture.TIM_ICFilter = 0xF;
	TIM_ICInit(Encoder_TimR, &ICInitStruture);
	ICInitStruture.TIM_Channel = Encoder_TimR_Channel_B;
	ICInitStruture.TIM_ICFilter = 0xF;
	TIM_ICInit(Encoder_TimR, &ICInitStruture);//ÓÒÂÖ±àÂëÆ÷ÉèÖÃ
	
	TIM_EncoderInterfaceConfig(Encoder_TimR, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(Encoder_TimR, ENABLE);	
	
}

int16_t Get_Encoder(int x)
{
	int16_t temp;
	switch(x)
	{
		case LEFT_ENCODER:
		{
			temp = TIM_GetCounter(Encoder_Tim);
			TIM_SetCounter(Encoder_Tim, 0);
			break;
		}
		case RIGHT_ENCODER:
		{
			temp = TIM_GetCounter(Encoder_TimR);
			TIM_SetCounter(Encoder_TimR, 0);
			break;
		}
	}

	return temp;
}

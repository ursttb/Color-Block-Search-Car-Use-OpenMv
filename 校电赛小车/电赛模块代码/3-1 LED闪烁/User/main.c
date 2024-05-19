#include "stm32f10x.h"                  // Device header
#include "Delay.h"

////USE_STDPERIPH_DRIVER

//int main()
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	while(1)
//	{
//		GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	}
//}

#include "stm32f10x.h"                  // Device header

#include "Delay.h"

uint8_t i = 0;
extern const unsigned char BGM1[][2];

int main()
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	//GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	
//	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//	//GPIO_SetBits(GPIOA, GPIO_Pin_5);
// 	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	while(1)
	{
		;
	}
	
	//return 0;
}


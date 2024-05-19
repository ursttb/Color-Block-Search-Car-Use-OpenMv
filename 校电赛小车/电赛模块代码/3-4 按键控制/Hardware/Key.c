#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define KEY_GPIO GPIOB
#define KEY_GPIO_PIN GPIO_Pin_1
#define RCC_KEY_GPIO RCC_APB2Periph_GPIOB

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_KEY_GPIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY_GPIO_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              
	GPIO_Init(KEY_GPIO, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(KEY_GPIO, KEY_GPIO_PIN) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(KEY_GPIO, KEY_GPIO_PIN) == 0);
		Delay_ms(20);
		KeyNum = 1;
	}
    
	return KeyNum;
}

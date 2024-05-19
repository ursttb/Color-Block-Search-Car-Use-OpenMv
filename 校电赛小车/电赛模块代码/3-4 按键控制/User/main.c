#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNum;
uint8_t Num;


int main(void)
{
	LED_Init();
	Key_Init();
	
	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
            Num=~Num;
           
		}
        if (Num)
        {
            LED1_ON();
        }
        else
            LED1_OFF();
        
	}
}

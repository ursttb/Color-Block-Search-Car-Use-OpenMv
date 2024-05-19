#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "uart.h"

uint8_t Length=0;
uint8_t color=0;
uint8_t x=0;
int main()
{
	OLED_Init();
	USARTx_Init();
	while(1)
	{
		Length=Send();
		color=colorsearch();
		x=x_position();
		OLED_ShowNum(2,1,Length,3);
		OLED_ShowNum(3,1,color,3);
		OLED_ShowNum(4,1,x,3);
	}
}

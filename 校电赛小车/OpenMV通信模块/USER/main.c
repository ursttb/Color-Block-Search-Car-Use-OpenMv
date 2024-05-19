/******************************
SDA:PA2
SCL:PA1
******************************/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "uart.h"
#include "stdio.h"


int main(void)
 {		

		OLED_Init();
		delay_init();	    	                              //��ʱ������ʼ��	  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		USARTx_Init();
		OLED_ColorTurn(0);          //0������ʾ��1 ��ɫ��ʾ
		OLED_DisplayTurn(0);        //0������ʾ 1 ��Ļ��ת��ʾ
	 
		while(1) 
		{
			OLED_Refresh();
	
		}
	
 }

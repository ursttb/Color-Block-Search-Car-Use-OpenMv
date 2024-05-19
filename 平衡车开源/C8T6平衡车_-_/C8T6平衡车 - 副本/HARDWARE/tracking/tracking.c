#include "tracking.h"
#include "sys.h"

/**************************************************************************
函数功能：循迹模块IO口初始化
入口参数：无
返回  值：无
**************************************************************************/
void Tracking_Init(void)
{
	
	//这一块的IO口配置，只要不更换IO口就不用改。
	 
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB时钟
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能A\B时钟
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PB 0 1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);
} 









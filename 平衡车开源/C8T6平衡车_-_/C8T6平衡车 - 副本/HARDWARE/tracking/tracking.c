#include "tracking.h"
#include "sys.h"

/**************************************************************************
�������ܣ�ѭ��ģ��IO�ڳ�ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void Tracking_Init(void)
{
	
	//��һ���IO�����ã�ֻҪ������IO�ھͲ��øġ�
	 
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOBʱ��
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��A\Bʱ��
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PB 0 1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
		GPIO_Init(GPIOB, &GPIO_InitStructure);
} 









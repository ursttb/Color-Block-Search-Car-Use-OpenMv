#include "sys.h"
#include "usart.h"	  
#include <stdio.h>
#include <string.h>  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


  
void usart_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

char usart_readbuff[30] = {0}; //���ڽ��ܻ�������
u8 usart_readok = 0; //һ֡���ݴ����־
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 temp;
	static u8 count = 0;  // ����������Ʊ���
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�ж��Ƿ�Ϊ�����ж�
	{
		temp = USART_ReceiveData(USART1);	//��ȡ���յ�������,������жϱ�־
		if(temp == '#' && usart_readok == 0) 
		{
			usart_readbuff[count] = '#';
			usart_readok = 1;
			count = 0;
		}
		else if(usart_readok==0)
		{
			usart_readbuff[count] = temp; //������յ������ݵ����ջ�������
			count++; //�����±��л�
			if(count >= 30) // ��ֹ����Խ��
				count = 0;
		}
	}
}


 

u8 usart_get_data(char *cmd,float *d1,float *d2,float *d3)
{
	u8 flag = 0;
	if(usart_readok == 1)
	{
		if(sscanf(usart_readbuff,"%3s=%f,%f,%f#",
											cmd,d1,d2,d3)==4)
		{
			flag = 1;
		}
		//���������ɱ�־
		memset(usart_readbuff,0,sizeof(usart_readbuff));
		usart_readok = 0;
	}
	return flag;
}


void usart_senddatas(USART_TypeDef* USARTx,u8* addr,int size)
{
	while(size--) //�ж����ݷ�����û��
	{
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);//�ȴ���һ��byte�����ݷ��ͽ�����
		USART_SendData(USARTx,*addr);//����STM32��׼�⺯����������
		addr++; //��ַƫ��
	}
}



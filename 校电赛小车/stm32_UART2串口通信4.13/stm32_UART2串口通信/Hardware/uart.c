#include "uart.h"
#include "oled.h"
#include "stdio.h"
#include "stm32f10x.h"                  // Device header

#define MY_USART USART2//����ѡ��
#define RCC_MY_USART RCC_APB1Periph_USART2//����ʹ��,�ǵ��ڴ����޸�ABPx
#define USART_Pin_TX GPIO_Pin_2//TX
#define USART_Pin_RX GPIO_Pin_3//RX
#define MY_USART_GPIO GPIOA//����GPIO��
#define RCC_MY_USART_GPIO RCC_APB2Periph_GPIOA//���� GPIO ʹ��
#define USART_IRQN USART2_IRQn//�ж�����

static u8 Cx=0,Cy=0,Cw=0,Ch=0;


void USARTx_Init(void)
{
	  //USART1_TX:PA 9   
	  //USART1_RX:PA10
		GPIO_InitTypeDef GPIO_InitStructure;     //���ڶ˿����ýṹ�����
		USART_InitTypeDef USART_InitStructure;   //���ڲ������ýṹ�����
		NVIC_InitTypeDef NVIC_InitStructure;     //�����ж����ýṹ�����

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //��PA�˿�ʱ��

    //USART1_TX   PA9
    GPIO_InitStructure.GPIO_Pin = USART_Pin_TX;          		 //PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  		 //�趨IO�ڵ�����ٶ�Ϊ50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		 //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);             	 	 //��ʼ��PA9
    //USART1_RX	  PA10
    GPIO_InitStructure.GPIO_Pin = USART_Pin_RX;             //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //��ʼ��PA10 

    //USART1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART_IRQN;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;  //��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		    //�����ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			      //IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	                          //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������
		USART_InitStructure.USART_BaudRate = 115200;                  //���ڲ�����Ϊ115200
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;        //һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;           //����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                  //�շ�ģʽ
    USART_Init(MY_USART, &USART_InitStructure);                     //��ʼ������1

    USART_ITConfig(MY_USART, USART_IT_RXNE, ENABLE); //ʹ���ж�
    USART_Cmd(MY_USART, ENABLE);                     //ʹ�ܴ���1

  	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
	  USART_ClearFlag(MY_USART, USART_FLAG_TC);        //�崮��1���ͱ�־
		
}

//USART1 ȫ���жϷ�����
void USART2_IRQHandler(void)			 
{
		u8 com_data; 
		u8 i;
		static u8 RxCounter1=0;
		static u16 RxBuffer1[10]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;


		if( USART_GetITStatus(MY_USART,USART_IT_RXNE)!=RESET)  	   //�����ж�  
		{
				USART_ClearITPendingBit(MY_USART,USART_IT_RXNE);   //����жϱ�־
				com_data = USART_ReceiveData(MY_USART);
			
				if(RxState==0&&com_data==0x2C)  //0x2c֡ͷ
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;OLED_Clear();
				}
		
				else if(RxState==1&&com_data==0x12)  //0x12֡ͷ
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1>=10||com_data == 0x5B)       //RxBuffer1��������,�������ݽ���
					{
						RxState=3;
						RxFlag1=1;
						Cx=RxBuffer1[RxCounter1-5];
						Cy=RxBuffer1[RxCounter1-4];
						Cw=RxBuffer1[RxCounter1-3];
						Ch=RxBuffer1[RxCounter1-2];

					}
				}
		
				else if(RxState==3)		//����Ƿ���ܵ�������־
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									USART_ITConfig(MY_USART,USART_IT_RXNE,DISABLE);//�ر�DTSABLE�ж�
									if(RxFlag1)
									{
//									OLED_Refresh();
//									OLED_ShowNum(0, 0,Cx,3,16,1);
//									OLED_ShowNum(0,17,Cy,3,16,1);
//									OLED_ShowNum(0,33,Cw,3,16,1);
//									OLED_ShowNum(0,49,Ch,3,16,1);
									}
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(MY_USART,USART_IT_RXNE,ENABLE);
									
						}
						else   //���մ���
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //�����������������
									}
						}
				} 
	
				else   //�����쳣
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<10;i++)
						{
								RxBuffer1[i]=0x00;      //�����������������
						}
				}

		}
		
}

uint8_t Send()
{
	return(Cw);
}
uint8_t colorsearch()
{
	return (Ch);
}
uint8_t x_position()
{
	return (Cx);
}
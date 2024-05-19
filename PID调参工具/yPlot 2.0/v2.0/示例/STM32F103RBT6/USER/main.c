#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
 
 
#include <math.h>
#include <string.h>



#define PI 3.1415f


float datas[4];
float cmd_P,cmd_I,cmd_D;


void send_wave(void);
void getdatas(void);
void get_cmd(void);

 int main(void)
 {	
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	usart_init(115200);
	while(1)
	{
		send_wave();
		getdatas();
		get_cmd();
	}
 }


void send_wave(void)
{
	//����ͨ����֡ͷ֡β
	u8 frameNameHead[] = "AABBCC";
	u8 frameNameEnd[] = "CCBBAA";
	
	//��������֡ͷ֡β
	u8 frameDataHead[] = "DDEEFF";
	u8 frameDataEnd[] = "FFEEDD";
	
	//����ͨ����
	u8 name[] = {"sin(x),cos(x),cos(2x),2cos(x),P,I,D"};
	
	//��ֵ����
	float channels[7];
	channels[0] = datas[0];
	channels[1] = datas[1];
	channels[2] = datas[2];
	channels[3] = datas[3];
	channels[4] = cmd_P;
	channels[5] = cmd_I;
	channels[6] = cmd_D;
	
	
	//ͨ������1������λ����������
	usart_senddatas(USART1,frameNameHead,sizeof(frameNameHead)-1);
	usart_senddatas(USART1,name,sizeof(name)-1);
	usart_senddatas(USART1,frameNameEnd,sizeof(frameNameEnd)-1);
	
	usart_senddatas(USART1,frameDataHead,sizeof(frameDataHead)-1);
	usart_senddatas(USART1,(u8*)channels,sizeof(channels));
	usart_senddatas(USART1,frameDataEnd,sizeof(frameDataEnd)-1);
	
}


void getdatas(void)
{
	static float x = 0.00f * PI;
	datas[0] = sinf(x);
	datas[1] = cosf(x);
	datas[2] = cosf(2 * x);
	datas[3] = 2 * cosf(x);
	
	x += 0.05f * PI;
}



void get_cmd(void)
{
	char u_buff[10];
	float u_d1,u_d2,u_d3;
	if(usart_get_data(u_buff,&u_d1,&u_d2,&u_d3))
	{
		if(strcmp(u_buff,"PID") == 0) //�Ƚ���������ַ��Ƿ�ΪPID
		{
			cmd_P = u_d1;
			cmd_I = u_d2;
			cmd_D = u_d3;
		}
	}
	memset(u_buff,0,sizeof(u_buff));
}


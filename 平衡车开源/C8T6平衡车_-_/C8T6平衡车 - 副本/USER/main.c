#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"		
#include "led.h"
#include "mpu6050.h"
#include "OLED.h"
#include "PWMA.h"
#include "motor.h"
#include	"encoder.h"
#include	"exti.h"
#include	"control.h"
#include "oled.h"
#include "SR04.h"

#define MAX_Speed 18 
short aacx,aacy,aacz;		//���ٶ�
short gyrox,gyroy,gyroz;	//������
float Pitch,Roll,Yaw; 		//ŷ����:������,��ת�ǣ�ƫ����
//int Moto1,Moto2;//�������
int PWM_MAX=7200,PWM_MIN=(-7200);	

float leng;
float Pitch,Roll,Yaw;	

int main(void)
{	
	
	u8 string[10] = {0};	
	LED_Init();
		

	delay_init();
	NVIC_Config();


	uart1_init(9600);
	
	SR04_Init();		//������ģ���Ӧ��IO�ڳ�ʼ��

	OLED_Init();
 OLED_ShowString(40,0,"MPU5060",16);
	OLED_Refresh();
	
	IIC_Init();
	MPU6050_initialize();     //=====MPU6050��ʼ��	
	DMP_Init();
	MPU6050_EXTI_Init();

	encoder_TIM3_Init();
	encoder_TIM4_Init(); 

  motor_init();
	PWM_Init(7200,0);
//	OLED_ShowString(40,0,"�����豸��ʼ���ɹ�",16);


	while(1)
	{		
		
		delay_ms(500);

		sprintf((char *)string,"leng:%.2f",leng);
		OLED_ShowString(16,0,string,16);

		sprintf((char *)string,"Pitch:%.2f",Pitch);
		OLED_ShowString(16,16,string,16);	
		sprintf((char *)string,"Roll :%.2f",Roll);
		OLED_ShowString(16,32,string,16);
		sprintf((char *)string,"Yaw  :%.2f",Yaw);
		OLED_ShowString(16,48,string,16);
		OLED_Refresh();
	}
}

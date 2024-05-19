#include "stm32f10x.h"                  // Device header
#include "SYS.h"

uint8_t Achievement = 0;
extern float AX,AZ;
extern int16_t Encoder_left, Encoder_right;
extern float Distance;
extern int PWM_Out;

int i = 0;

int motor1 = 999;int motor2 = -999;

uint64_t numlen(uint64_t num)//�������ֵĳ���
{
    uint64_t len = 1;        // ��ʼ����Ϊ1
    for(; num > 9; ++len)    // �ж�num�Ƿ����9�����򳤶�+1
        num /= 10;	         // ʹ�ó����������㣬ֱ��numС��1
    return len;              // ���س��ȵ�ֵ
}

int main()
{
	OLED_Init();
	
	//USARTx_Init();
	
	MPU6050_Config();
	
	
	Encoder_Init();
	HC_SR04_Init();
	PWM_Init();
	Motot_Init();

	EXIT_Init();
	PID_Init();                                                                                                                                                                                                       
	
	Motor_PWMLoad(&motor1, &motor1);
	while(1)
	{
		OLED_ShowSignedNum(1,1,AX,5);		
		OLED_ShowSignedNum(2,1,Encoder_left,3);
		OLED_ShowSignedNum(2,6,Encoder_right,3);
		OLED_ShowNum(3, 1,Distance,numlen(Distance));		//��ʾ��λΪ���׵ľ�����
		OLED_ShowNum(4,1,PWM_Out,3);
	}
 	return 0;
} 

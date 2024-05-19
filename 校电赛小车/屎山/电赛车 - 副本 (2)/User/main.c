#include "stm32f10x.h"                  // Device header
#include "SYS.h"

uint8_t Achievement = 0;
extern float AX,AZ;
extern int16_t Encoder_left, Encoder_right;
extern float Distance;
extern int PWM_Out;

int i = 0;

int motor1 = 999;int motor2 = -999;

uint64_t numlen(uint64_t num)//计算数字的长度
{
    uint64_t len = 1;        // 初始长度为1
    for(; num > 9; ++len)    // 判断num是否大于9，否则长度+1
        num /= 10;	         // 使用除法进行运算，直到num小于1
    return len;              // 返回长度的值
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
		OLED_ShowNum(3, 1,Distance,numlen(Distance));		//显示单位为毫米的距离结果
		OLED_ShowNum(4,1,PWM_Out,3);
	}
 	return 0;
} 

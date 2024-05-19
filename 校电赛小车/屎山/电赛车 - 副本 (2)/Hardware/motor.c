#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "MOTOR.h"

//��������PWM�޷�
#define PWM_MAX 7200
#define PWM_MIN -7200

void Motot_Init()
{
	RCC_APB2PeriphClockCmd(RCC_MOTOR_GPIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MOTOR_AIN1 | MOTOR_BIN1 | MOTOR_CIN1 | MOTOR_DIN1;//ֻ��ʼ��IN1�ţ���ΪIN2���Ѿ���PWM���ó�ʼ��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOT_GPIO, &GPIO_InitStructure);

}
void Motor_PWMLoad(int* motor1, int* motor2)//�˴�motor1���������������, motor2�����ұ���������
{
	int temp = 0;
	
	//motor12����������PWM����Ҫ���һ���޷��޸�
	if(*motor1>PWM_MAX)	*motor1=PWM_MAX;
	if(*motor1<PWM_MIN)	*motor1=PWM_MIN;
	
	if(*motor2>PWM_MAX)	*motor2=PWM_MAX;
	if(*motor2<PWM_MIN)	*motor2=PWM_MIN;

	if(*motor1>0)//��ת
	{
		GPIO_SetBits(MOTOT_GPIO, MOTOR_AIN1); 
		GPIO_SetBits(MOTOT_GPIO, MOTOR_BIN1);
		OCA_PolarityConfig(PWM_TIM, TIM_OCPolarity_Low);
		OCB_PolarityConfig(PWM_TIM, TIM_OCPolarity_Low);
		temp = *motor1;
	}
	else//��ת
	{
		GPIO_ResetBits(MOTOT_GPIO, MOTOR_AIN1); 
		GPIO_ResetBits(MOTOT_GPIO, MOTOR_BIN1); 
		OCA_PolarityConfig(PWM_TIM, TIM_OCPolarity_High);
		OCB_PolarityConfig(PWM_TIM, TIM_OCPolarity_High);
		temp = *motor1; temp = -temp;
	}
	OCA_SetCompare(PWM_TIM, temp); OCB_SetCompare(PWM_TIM, temp);
	
	if(*motor2>0)//��ת
	{
		GPIO_SetBits(MOTOT_GPIO, MOTOR_CIN1); 
		GPIO_ResetBits(MOTOT_GPIO, MOTOR_DIN1);
		OCC_PolarityConfig(PWM_TIM, TIM_OCPolarity_Low);
		OCD_PolarityConfig(PWM_TIM, TIM_OCPolarity_High);
		temp = *motor2;
	}
	else//��ת
	{
		GPIO_ResetBits(MOTOT_GPIO, MOTOR_CIN1); 
		GPIO_SetBits(MOTOT_GPIO, MOTOR_DIN1); 
		OCC_PolarityConfig(PWM_TIM, TIM_OCPolarity_High);
		OCD_PolarityConfig(PWM_TIM, TIM_OCPolarity_Low);
		temp = *motor2; temp = -temp;
	}
	OCC_SetCompare(PWM_TIM, temp); OCD_SetCompare(PWM_TIM, temp);
	
}

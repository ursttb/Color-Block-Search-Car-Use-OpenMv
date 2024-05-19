#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//电机转向GPIO端口,最小系统板中GPIOB是四路AIN1的端口
#define MOTOT_GPIO GPIOB
#define RCC_MOTOR_GPIO RCC_APB2Periph_GPIOB

//电机1
#define MOTOR_AIN1 GPIO_Pin_12
#define MOTOR_AIN2 PWM_A_TIM_GPIO_Pin//AIN2脚交给PWM控速

//电机2
#define MOTOR_BIN1 GPIO_Pin_10
#define MOTOR_BIN2 PWM_B_TIM_GPIO_Pin

//电机3
#define MOTOR_CIN1 GPIO_Pin_1
#define MOTOR_CIN2 PWM_C_TIM_GPIO_Pin

//电机4
#define MOTOR_DIN1 GPIO_Pin_0
#define MOTOR_DIN2 PWM_D_TIM_GPIO_Pin


//最后输出的PWM限幅
#define PWM_MAX 7200
#define PWM_MIN -7200

void Motot_Init()
{
	RCC_APB2PeriphClockCmd(RCC_MOTOR_GPIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MOTOR_AIN1 | MOTOR_BIN1 | MOTOR_CIN1 | MOTOR_DIN1;//只初始化IN1脚，因为IN2脚已经被PWM复用初始化
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOT_GPIO, &GPIO_InitStructure);

}
void Motor_PWMLoad(int* motor1, int* motor2)//此处motor1控制左边两个轮子, motor2控制右边两个轮子
{
	int temp = 0;
	
	//motor12是最后输出的PWM，需要最后一次限幅修改
	if(*motor1>PWM_MAX)	*motor1=PWM_MAX;
	if(*motor1<PWM_MIN)	*motor1=PWM_MIN;
	
	if(*motor2>PWM_MAX)	*motor2=PWM_MAX;
	if(*motor2<PWM_MIN)	*motor2=PWM_MIN;

	if(*motor1>0)//正转
	{
		GPIO_ResetBits(MOTOT_GPIO, MOTOR_AIN1); OCA_PolarityConfig(PWM_TIM, TIM_OCPolarity_High);
		temp = *motor1;
	}
	else//反转
	{
		GPIO_SetBits(MOTOT_GPIO, MOTOR_AIN1); OCA_PolarityConfig(PWM_TIM, TIM_OCPolarity_Low);
		temp = *motor1; temp = -temp;
	}
	OCA_SetCompare(PWM_TIM, temp);
	
//	if(motor2>0)//正转
//	{
//		GPIO_SetBits(MOTOT_GPIO, MOTOR_IN3); GPIO_ResetBits(MOTOT_GPIO, MOTOR_IN4);
//	}
//	else//反转
//	{
//		GPIO_SetBits(MOTOT_GPIO, MOTOR_IN4); GPIO_ResetBits(MOTOT_GPIO, MOTOR_IN3);
//	}
	OCB_SetCompare(PWM_TIM, *motor2);
}

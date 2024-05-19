#ifndef __MOTOR_H
#define __MOTOR_H


//电机转向GPIO端口,最小系统板中GPIOB是四路AIN1的端口
#define MOTOT_GPIO GPIOB
#define RCC_MOTOR_GPIO RCC_APB2Periph_GPIOB

//电机1(左后轮)
#define MOTOR_AIN1 GPIO_Pin_11
#define MOTOR_AIN2 PWM_A_TIM_GPIO_Pin//AIN2脚交给PWM控速

//电机2(左前轮)
#define MOTOR_BIN1 GPIO_Pin_1
#define MOTOR_BIN2 PWM_B_TIM_GPIO_Pin

//电机3(右前轮)
#define MOTOR_CIN1 GPIO_Pin_10
#define MOTOR_CIN2 PWM_C_TIM_GPIO_Pin

//电机4(右后轮)
#define MOTOR_DIN1 GPIO_Pin_0
#define MOTOR_DIN2 PWM_D_TIM_GPIO_Pin


void Motot_Init(void);
void Motor_PWMLoad(int* motor1, int* motor2);//此处motor1控制左边两个轮子, motor2控制右边两个轮子
void turn(int direction, int angle);	

#endif

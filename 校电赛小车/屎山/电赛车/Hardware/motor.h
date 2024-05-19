#ifndef __MOTOR_H
#define __MOTOR_H


//���ת��GPIO�˿�,��Сϵͳ����GPIOB����·AIN1�Ķ˿�
#define MOTOT_GPIO GPIOB
#define RCC_MOTOR_GPIO RCC_APB2Periph_GPIOB

//���1(�����)
#define MOTOR_AIN1 GPIO_Pin_11
#define MOTOR_AIN2 PWM_A_TIM_GPIO_Pin//AIN2�Ž���PWM����

//���2(��ǰ��)
#define MOTOR_BIN1 GPIO_Pin_1
#define MOTOR_BIN2 PWM_B_TIM_GPIO_Pin

//���3(��ǰ��)
#define MOTOR_CIN1 GPIO_Pin_10
#define MOTOR_CIN2 PWM_C_TIM_GPIO_Pin

//���4(�Һ���)
#define MOTOR_DIN1 GPIO_Pin_0
#define MOTOR_DIN2 PWM_D_TIM_GPIO_Pin


void Motot_Init(void);
void Motor_PWMLoad(int* motor1, int* motor2);//�˴�motor1���������������, motor2�����ұ���������
void turn(int direction, int angle);	

#endif

#ifndef __CONTROL_H
#define __CONTROL_H
#include	"sys.h"
#include	"exti.h"
#include	"encoder.h"
#include	"motor.h"
#include "sys.h"
#include "PWMA.h"


void EXTI9_5_IRQHandler(void);


//ֱ����
int Vertical(float Expect_Angle,float Real_Angle,float gyro_Y);

//�ٶȻ�
int Velocity(int target,int encoder_left,int encoder_right);

//ת��
int Turn(int gyro_Z,int turn_angle);


#endif




#include "stm32f10x.h"                  // Device header

extern uint8_t Achievement;

//��е��ֵ
float Med_Displacement = 0;

//�⻷λ�ƻ�����
float 
	Displacement_Kp = 0,
	Displacement_Ki = 0;

//�ڻ��ٶȻ�����(����ʹ�ýṹ�壬�������ﲻ��)
float 
	Velocity_Kp = 0, 
	Velocity_Kd = 0;


//�⻷λ�ƻ�����
//��ʽ:PID_XOUT = Kp*XE+ Ki*XE_I���������Kp*λ��ƫ��+Ki*XEλ��ƫ��Ļ��֣�
//����:����λ��(��е��ֵ)��SR04�������
int Displacement(float Med, float SR04_Num)
{
	static int  PWM_Out, Displacement_Err, Displacement_Low_Err, Displacement_Low_Err_last, Displacement_Err_S;
	float a = 0.7;//�˲�����
	
	//1.����λ��ƫ��
	Displacement_Err = SR04_Num - Med;
	
	//�˴�������Ҫһ����ͨ�˲�
	Displacement_Low_Err = (1-a)*Displacement_Err + a*Displacement_Low_Err_last;
	Displacement_Low_Err_last = Displacement_Low_Err;	
	
	//2.����λ��ƫ�����
	Displacement_Err_S += Displacement_Low_Err;
	//3.�����޷�
	Displacement_Err_S = Displacement_Err_S>10000?10000:(Displacement_Err_S<(-10000)?(-10000):Displacement_Err_S);
	//4.λ�ƻ��������
	PWM_Out = Displacement_Kp*Displacement_Low_Err + Displacement_Ki*Displacement_Err_S;
	
	return PWM_Out;
}

//PID����
//�ڻ��ٶȻ�����
//��ʽ:PID_VOUT = Kp*VE + Kd*VE_D���������Kp*�ٶ�ƫ��+Kd*���ٶ�ƫ�
//����:�����ٶ�(ʵ�������⻷��������)��������AB���ٶȡ���ʵ���ٶ�
int Velocity(float Med, float Encode_A, float Encode_B, float Acc)
{
	static int PWM_Out, Velocity_Err, Velocity_Low_Err, Velocity_Low_Err_last;
	float a = 0.7;//��ͨ�˲�����
	
	//����һ�������ٶ�ƫ�����˲��������������˲��õ���ʵ�ٶ������ٶ�ƫ��˴��Ȳ��÷�������
	
//	//1.�����ٶ�ƫ������ԣ�
//	Velocity_Err = (Encode_A + Encode_B)/2;
//	//2.���ٶ�ƫ����е�ͨ�˲�
//	Velocity_Low_Err = (1-a)*Velocity_Err + a*Velocity_Low_Err_last;
//	Velocity_Low_Err_last = Velocity_Low_Err;
	
	//1.����ʵ���ٶ�
	Velocity_Err = (Encode_A + Encode_B);
	//2.���ٶȽ��е�ͨ�˲�
	Velocity_Low_Err = (1-a)*Velocity_Err + a*Velocity_Low_Err_last;
	Velocity_Low_Err_last = Velocity_Low_Err;	
	//3.�����ٶ�ƫ��
	Velocity_Err = Velocity_Err - Med;
	//4.���빫ʽ
	PWM_Out = Velocity_Kp * Velocity_Low_Err  + Velocity_Kd * (Acc - 0);
	
	return PWM_Out;
}

//PID����
//����PID���ս��
//��ʽ:�ٶȻ�Kp*��ʵ�ٶ� + Kd*�ٶ�ƫ��΢�� - �ٶȻ�Kp*��λ�ƻ�Kp*λ��ƫ�� + Ki*λ��ƫ��΢�֣�
//����:λ�ƻ���������������AB���ٶ�(�������ٶ�ƫ��)����ʵ���ٶ�(�����ٶ�ƫ��΢��)
int Final_PWM_Out(float Displacement_PWM_Out, float Encode_A, float Encode_B, float Acc)
{
	static int PWM_Out, Velocity_reality, Velocity_Low_reality , Velocity_Low_reality_last;
	float a = 0.7;//��ͨ�˲�����
	
	//1.����ʵ���ٶ�
	Velocity_reality  = (Encode_A + Encode_B);
	//2.���ٶȽ��е�ͨ�˲�
	Velocity_Low_reality  = (1-a)*Velocity_reality + a*Velocity_Low_reality_last;
	Velocity_Low_reality_last = Velocity_Low_reality ;	
	//3.���㴮��PID���ս��
	PWM_Out = Velocity_Kp*Velocity_Low_reality + Velocity_Kd*Acc - Velocity_Kp*Displacement_PWM_Out;
	
	//����Ҫ���һ���޷���δд��
	
	return PWM_Out;
}

void Set_Med_Displacement(int* Med_Displacement)
{
	switch(Achievement)
	{
		case 0://ȥA
		{
			*Med_Displacement = 5;
			break;
		}
		case 1://������㣬ת��
		{
			*Med_Displacement = 100;
			break;
		}
		case 2://ȥB
		{
			*Med_Displacement = 5;
			break;
		}
		case 3://����15��ת��
		{
			*Med_Displacement = 15;
			break;
		}
		case 4://ȥC
		{
			*Med_Displacement = 5;
			break;
		}
		case 5://����55���ص�BC��ֱ���㴦ת�䣬��ͷ����B
		{
			*Med_Displacement = 55;
			break;
		}
		case 6://�������
		{
			*Med_Displacement = 100;
			break;
		}
	}

}
#include "stm32f10x.h"                  // Device header
#include "SYS.h"
extern int motor1;
extern int motor2;

uint8_t Mode = 1;
//��ɫ����x����
uint8_t Red_x = 0;
//�ж����޺�ɫ����
uint8_t Redflag = 0;	
//�Ƕ�
int16_t angle; int16_t turn_dir;
//���ٶ�
float AX, AY, AZ; int16_t GX, GY, GZ;
//�������ٶ�
int16_t Encoder_left, Encoder_right;
//λ��
float Distance;
//�����ֵ
int Out_Displacement,Out_Velocity;//λ�ƻ����ֵ���ٶȻ����ֵ
int PWM_Out;//���������ֵ

extern uint8_t Achievement;

//��е��ֵ
float Med_Displacement;

//�⻷λ�ƻ�����
float
	Displacement_Kp = 100,
	Displacement_Ki = 0;

//�ڻ��ٶȻ�����(����ʹ�ýṹ�壬�������ﲻ��)
float
	Velocity_Kp = 0, 
	Velocity_Kd = 0;

void PID_Init()
{
	RCC_APB1PeriphClockCmd(RCC_PID_TIM, ENABLE);//�ö�ʱ��2��PID��ʱ�ж�
	
	TIM_InternalClockConfig(PID_TIM);
	
	TIM_TimeBaseInitTypeDef TimeBaseInitStruture;
	TimeBaseInitStruture.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBaseInitStruture.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStruture.TIM_Period = 100  - 1;
	TimeBaseInitStruture.TIM_Prescaler = 7200 - 1;
	TimeBaseInitStruture.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(PID_TIM, &TimeBaseInitStruture);
	
	TIM_ClearFlag(PID_TIM, TIM_FLAG_Update);
	TIM_ITConfig(PID_TIM, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(PID_TIM,ENABLE);
}

void PID_TIM_IRQHandler()
{
	if(TIM_GetITStatus(PID_TIM, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(PID_TIM, TIM_IT_Update);
		
		//1.�������ݶ�ȡ
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);//����ٶ�
		Encoder_left = Get_Encoder(LEFT_ENCODER);//���ٶ�
		Encoder_right = Get_Encoder(RIGHT_ENCODER);
		Distance = Send(); //�����
		Red_x = x_position();//���ɫ����x����
		
		//2.����ѹ��PID���м���
		Out_Displacement = Displacement(Med_Displacement, Distance);
		Out_Velocity = Velocity(Out_Displacement, Encoder_left, Encoder_right, AX);
		PWM_Out = Final_PWM_Out(Out_Displacement, Encoder_left, Encoder_right, AX);		
		PWM_Out = -PWM_Out;
		
		if(colorsearch())
		{
			Redflag = 1;
		}
		else Redflag = 0;
		
		if(Red_x <105 || Red_x > 115)//���ձ�־λ
		{
			//�ж�ת�ķ���
			if(Red_x < 105)
			{
				turn_dir = -1;
			}
			else if(Red_x > 115)
			{
				turn_dir = 1;
			}
			else
			{
				turn_dir = 0;
			}
			TIM_Cmd(PID_TIM,DISABLE); Mode = 2; 
		}
		
		//3.PID�������PWM������
		Motor_PWMLoad(&PWM_Out, &PWM_Out);
	
	}
}

#if 0
/*
int Out_Displacement,Out_Velocity;//����������Ӧ�����ֵ
int Distance;//����
int Encoder_left,Encoder_right;//����������
int Acc;//���ٶ�

{
	1.�������ݶ�ȡ
	Distance = //�����
	Encoder_left = ; Encoder_right = ;//���ٶ�
	Acc//����ٶ�

	2.����ѹ��PID���м���
	Out_Displacement = Displacement(Med_Displacement, Distance);
	Out_Velocity = Velocity(Out_Displacement, Encoder_left, Encoder_right, Acc);
	PWM_Out = Final_PWM_Out(Out_Displacement, Encoder_left, Encoder_right, Acc);

	3.PID�������PWM������
	Motor_PWMLoad
}
*/
#endif

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
int Velocity(float Med, int16_t Encode_A, int16_t Encode_B, float Acc)
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
	Velocity_Low_Err = Velocity_Low_Err - Med;
	//4.���빫ʽ
	PWM_Out = Velocity_Kp * Velocity_Low_Err  + Velocity_Kd * (Acc - 0);
	
	return PWM_Out;
}

//PID����
//����PID���ս��
//��ʽ:�ٶȻ�Kp*��ʵ�ٶ� + Kd*�ٶ�ƫ��΢�� - �ٶȻ�Kp*��λ�ƻ�Kp*λ��ƫ�� + Ki*λ��ƫ��΢�֣�
//����:λ�ƻ���������������AB���ٶ�(�������ٶ�ƫ��)����ʵ���ٶ�(�����ٶ�ƫ��΢��)
int Final_PWM_Out(float Displacement_PWM_Out, int16_t Encode_A, int16_t Encode_B, float Acc)
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

void Set_Med_Displacement(float* Med_Displacement)
{
	switch(Achievement)
	{
		case 0://ȥA
		{
			*Med_Displacement = 6;
			break;
		}
		case 1://������㣬ת��
		{
			*Med_Displacement = 170;
			//ת��
			//Motor_PWMLoad(&PWM_Out, &PWM_Out);
			//delay()
			//ͣת			
			break;
		}
		case 2://ȥB
		{
			*Med_Displacement = 6;
			break;
		}
		case 3://����15��ת��
		{
			*Med_Displacement = 15;
			break;
		}
		case 4://ȥC
		{
			*Med_Displacement = 6;
			break;
		}
		case 5://����55���ص�BC��ֱ���㴦ת�䣬��ͷ����B
		{
			*Med_Displacement = 55;
			break;
		}
		case 6://�������
		{
			*Med_Displacement = 170;
			break;
		}
	}

}

#include"control.h"
#include "SR04.h"
#include "delay.h"

int Moto1,Moto2;//�������

int Out_Vertical,Out_Velocity,Out_Turn;//��������Ӧ�����ֵ
int Encoder_left,Encoder_right;//����������

//���������ο���������Լ�����
float 
	target_speed=0,
	turn_speed=5;

//�����Ƕ�
float expect_angle=-2;	
//0.7

//ֱ������Kp��Kd
float 
	Vertical_Kp=-178,
//-280,-168 
	Vertical_Kd=1.432;
//2.38,1.47

//�ٶȻ���KP��Kd
float 
	Velocity_Kp=-0.45,
//-0.45,-0.00225
	Velocity_Ki=-0.00225;

float
turn_kd=0.8,
turn_kp=20;  

#define MAX_Speed 22 
#define MAX_Turn_Speed 80
                             
                             
void EXTI9_5_IRQHandler(void)
{   
	u32 t;

	if(EXTI_GetITStatus(EXTI_Line5)!=0)
	{		
		int PWM_out;
		if(INT==0)
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
			Encoder_left = -Read_Speed(3);
			Encoder_right= Read_Speed(4);
			
			//����MPU�����ݿ���ж�ȡ��Ӧֵ
			Read_DMP(&Pitch,&Roll,&Yaw);
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�����Ƕ���ȡ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//���ٶȶ�ȡ


			t=Read_TIM1Distane();
			leng=1.0*t/58.0;        



		//����ģʽ
			
			if (Mode==2){
//					t=Read_TIM1Distane();
//					leng=1.0*t/58.0;
			if(leng<=40)
			{
				target_speed=-20;

			}
	
			else target_speed=0;}

			
			//��������С��ģʽ	
			if(Mode==1)		
			{					//����С��ǰ������
				if((Go==0)&&(Back==0))	
				{
					if(target_speed>0)
					{
						target_speed--;
					}
					else if(target_speed<0)
					{
						target_speed++;
					}
					else if	(target_speed==0)
					{
						target_speed=0;
					}
				}
			
				if		(Go==1)								target_speed++;
				if		(Back==1)							target_speed=-15;
						
				target_speed =	target_speed	>	MAX_Speed	?	MAX_Speed:(target_speed<(-MAX_Speed)?(-MAX_Speed):target_speed);//�޷�
						
				//����С������ת��
				if  	((Left==0)&&(Right==0))  turn_speed=0;
				if		(Left==1)								 turn_speed--;
				if		(Right==1)							 turn_speed++;		
				turn_speed 	=	turn_speed	>	MAX_Turn_Speed	?	MAX_Turn_Speed:(turn_speed<(-MAX_Turn_Speed)?(-MAX_Turn_Speed):turn_speed);//�޷�

				if			((Left==0)&&(Right==0))	turn_kd=0.5;//��������ת��ָ�����ת��Լ��,��֮
				else if	((Left==1)||(Right==1))	turn_kd=0;
			}			
			

	
			//��ȡ�����������ֵ
			Out_Velocity  =	 Velocity(target_speed,Encoder_left,Encoder_right);
			Out_Vertical  =  Vertical(Out_Velocity+expect_angle,Pitch,gyroy);
			Out_Turn	    =	 Turn(gyroz,turn_speed);
			
			PWM_out				=	 Out_Vertical-Vertical_Kp*Out_Velocity;
		
			
			Moto1	= PWM_out-Out_Turn;
			Moto2	= PWM_out+Out_Turn;
			Limit(&Moto1,&Moto2);
			Load(Moto1,Moto2);
			Stop(&expect_angle,&Pitch);
		}
				
	}
}



/***************
		�ǶȻ�PD����

***************/
int Vertical(float Expect_Angle,float Real_Angle,float gyro_Y)//�ֱ���������Ƕȣ���ʵ�Ƕȣ����ٶ�
{
		int PWM_out;
		
	
		PWM_out=Vertical_Kp*( Real_Angle-Expect_Angle)  + Vertical_Kd*( gyro_Y - 0 );

		return PWM_out;
}

/***************
	�ٶȻ�PI����

***************/
int Velocity(int target,int encoder_left,int encoder_right)	
{
		static int PWM_out,Encoder_value,Encoder_Sum_value;
		static int Encoder_value_low_out,Encoder_value_low_out_last;
		float a=0.7;
		Encoder_value	=	(encoder_left+encoder_right)	-	target;
		//��ͨ�˲�,��ֹ�ٶȿ��ƶ�ֱ����ɸ���
		Encoder_value_low_out = (1-a) * Encoder_value+	a*Encoder_value_low_out_last;
	
		Encoder_value_low_out_last	=		Encoder_value_low_out;
	
	//���ٶ�ƫ����л������
	Encoder_Sum_value+=Encoder_value_low_out;
	
	Encoder_Sum_value=Encoder_Sum_value>10000?10000:(Encoder_Sum_value<(-10000)?(-10000):Encoder_Sum_value);

	PWM_out	=	Velocity_Kp*Encoder_value_low_out	+	Velocity_Ki*Encoder_Sum_value;
	
		return PWM_out;
}

/*************** 
	ת�򻷣�Z��Ľ��ٶ�
***************/
int Turn(int gyro_Z,int turn_angle)
{
	int PWM_out;
	
	PWM_out=turn_kd*gyro_Z  +	turn_kp*turn_angle;
	
	return PWM_out;
}


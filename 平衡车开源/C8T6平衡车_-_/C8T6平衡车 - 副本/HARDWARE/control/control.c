#include"control.h"
#include "SR04.h"
#include "delay.h"

int Moto1,Moto2;//两个电机

int Out_Vertical,Out_Velocity,Out_Turn;//三个环对应的输出值
int Encoder_left,Encoder_right;//两个编码电机

//参数仅供参考，具体得自己调试
float 
	target_speed=0,
	turn_speed=5;

//期望角度
float expect_angle=-2;	
//0.7

//直立环的Kp与Kd
float 
	Vertical_Kp=-178,
//-280,-168 
	Vertical_Kd=1.432;
//2.38,1.47

//速度环的KP与Kd
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
			
			//利用MPU的数据库进行读取对应值
			Read_DMP(&Pitch,&Roll,&Yaw);
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//陀螺仪都读取
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//加速度读取


			t=Read_TIM1Distane();
			leng=1.0*t/58.0;        



		//避障模式
			
			if (Mode==2){
//					t=Read_TIM1Distane();
//					leng=1.0*t/58.0;
			if(leng<=40)
			{
				target_speed=-20;

			}
	
			else target_speed=0;}

			
			//蓝牙控制小车模式	
			if(Mode==1)		
			{					//控制小车前进后退
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
						
				target_speed =	target_speed	>	MAX_Speed	?	MAX_Speed:(target_speed<(-MAX_Speed)?(-MAX_Speed):target_speed);//限幅
						
				//控制小车左右转向
				if  	((Left==0)&&(Right==0))  turn_speed=0;
				if		(Left==1)								 turn_speed--;
				if		(Right==1)							 turn_speed++;		
				turn_speed 	=	turn_speed	>	MAX_Turn_Speed	?	MAX_Turn_Speed:(turn_speed<(-MAX_Turn_Speed)?(-MAX_Turn_Speed):turn_speed);//限幅

				if			((Left==0)&&(Right==0))	turn_kd=0.5;//若无左右转向指令，则开启转向约束,反之
				else if	((Left==1)||(Right==1))	turn_kd=0;
			}			
			

	
			//提取三个环的输出值
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
		角度环PD控制

***************/
int Vertical(float Expect_Angle,float Real_Angle,float gyro_Y)//分别代表期望角度，真实角度，角速度
{
		int PWM_out;
		
	
		PWM_out=Vertical_Kp*( Real_Angle-Expect_Angle)  + Vertical_Kd*( gyro_Y - 0 );

		return PWM_out;
}

/***************
	速度环PI控制

***************/
int Velocity(int target,int encoder_left,int encoder_right)	
{
		static int PWM_out,Encoder_value,Encoder_Sum_value;
		static int Encoder_value_low_out,Encoder_value_low_out_last;
		float a=0.7;
		Encoder_value	=	(encoder_left+encoder_right)	-	target;
		//低通滤波,防止速度控制对直立造成干扰
		Encoder_value_low_out = (1-a) * Encoder_value+	a*Encoder_value_low_out_last;
	
		Encoder_value_low_out_last	=		Encoder_value_low_out;
	
	//对速度偏差进行积分求和
	Encoder_Sum_value+=Encoder_value_low_out;
	
	Encoder_Sum_value=Encoder_Sum_value>10000?10000:(Encoder_Sum_value<(-10000)?(-10000):Encoder_Sum_value);

	PWM_out	=	Velocity_Kp*Encoder_value_low_out	+	Velocity_Ki*Encoder_Sum_value;
	
		return PWM_out;
}

/*************** 
	转向环：Z轴的角速度
***************/
int Turn(int gyro_Z,int turn_angle)
{
	int PWM_out;
	
	PWM_out=turn_kd*gyro_Z  +	turn_kp*turn_angle;
	
	return PWM_out;
}


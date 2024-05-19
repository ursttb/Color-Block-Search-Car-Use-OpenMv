#include "stm32f10x.h"                  // Device header
#include "SYS.h"
extern int motor1;
extern int motor2;

uint8_t Mode = 1;
//红色方块x坐标
uint8_t Red_x = 0;
//判断有无红色方块
uint8_t Redflag = 0;	
//角度
int16_t angle; int16_t turn_dir;
//加速度
float AX, AY, AZ; int16_t GX, GY, GZ;
//编码器速度
int16_t Encoder_left, Encoder_right;
//位移
float Distance;
//输出环值
int Out_Displacement,Out_Velocity;//位移环输出值和速度环输出值
int PWM_Out;//最终输出环值

extern uint8_t Achievement;

//机械中值
float Med_Displacement;

//外环位移环参数
float
	Displacement_Kp = 100,
	Displacement_Ki = 0;

//内环速度环参数(可以使用结构体，但我这里不用)
float
	Velocity_Kp = 0, 
	Velocity_Kd = 0;

void PID_Init()
{
	RCC_APB1PeriphClockCmd(RCC_PID_TIM, ENABLE);//用定时器2做PID定时中断
	
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
		
		//1.各个数据读取
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);//测加速度
		Encoder_left = Get_Encoder(LEFT_ENCODER);//测速度
		Encoder_right = Get_Encoder(RIGHT_ENCODER);
		Distance = Send(); //测距离
		Red_x = x_position();//测红色方块x坐标
		
		//2.数据压入PID进行计算
		Out_Displacement = Displacement(Med_Displacement, Distance);
		Out_Velocity = Velocity(Out_Displacement, Encoder_left, Encoder_right, AX);
		PWM_Out = Final_PWM_Out(Out_Displacement, Encoder_left, Encoder_right, AX);		
		PWM_Out = -PWM_Out;
		
		if(colorsearch())
		{
			Redflag = 1;
		}
		else Redflag = 0;
		
		if(Red_x <105 || Red_x > 115)//接收标志位
		{
			//判断转的方向
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
		
		//3.PID结果送入PWM处理电机
		Motor_PWMLoad(&PWM_Out, &PWM_Out);
	
	}
}

#if 0
/*
int Out_Displacement,Out_Velocity;//两个个环对应的输出值
int Distance;//距离
int Encoder_left,Encoder_right;//两个编码电机
int Acc;//加速度

{
	1.各个数据读取
	Distance = //测距离
	Encoder_left = ; Encoder_right = ;//测速度
	Acc//测加速度

	2.数据压入PID进行计算
	Out_Displacement = Displacement(Med_Displacement, Distance);
	Out_Velocity = Velocity(Out_Displacement, Encoder_left, Encoder_right, Acc);
	PWM_Out = Final_PWM_Out(Out_Displacement, Encoder_left, Encoder_right, Acc);

	3.PID结果送入PWM处理电机
	Motor_PWMLoad
}
*/
#endif

//外环位移环函数
//公式:PID_XOUT = Kp*XE+ Ki*XE_I（输出等于Kp*位移偏差+Ki*XE位移偏差的积分）
//输入:期望位移(机械中值)、SR04测距数据
int Displacement(float Med, float SR04_Num)
{
	static int  PWM_Out, Displacement_Err, Displacement_Low_Err, Displacement_Low_Err_last, Displacement_Err_S;
	float a = 0.7;//滤波参数
	
	//1.计算位移偏差
	Displacement_Err = SR04_Num - Med;
	
	//此处可能需要一个低通滤波
	Displacement_Low_Err = (1-a)*Displacement_Err + a*Displacement_Low_Err_last;
	Displacement_Low_Err_last = Displacement_Low_Err;	
	
	//2.计算位移偏差积分
	Displacement_Err_S += Displacement_Low_Err;
	//3.积分限幅
	Displacement_Err_S = Displacement_Err_S>10000?10000:(Displacement_Err_S<(-10000)?(-10000):Displacement_Err_S);
	//4.位移环输出计算
	PWM_Out = Displacement_Kp*Displacement_Low_Err + Displacement_Ki*Displacement_Err_S;
	
	return PWM_Out;
}

//PID函数
//内环速度环函数
//公式:PID_VOUT = Kp*VE + Kd*VE_D（输出等于Kp*速度偏差+Kd*加速度偏差）
//输入:期望速度(实际上是外环的输出结果)、编码器AB相速度、真实加速度
int Velocity(float Med, int16_t Encode_A, int16_t Encode_B, float Acc)
{
	static int PWM_Out, Velocity_Err, Velocity_Low_Err, Velocity_Low_Err_last;
	float a = 0.7;//低通滤波参数
	
	//方案一：先求速度偏差再滤波；方案二：先滤波得到真实速度再求速度偏差（此处先采用方案二）
	
//	//1.计算速度偏差（待调试）
//	Velocity_Err = (Encode_A + Encode_B)/2;
//	//2.对速度偏差进行低通滤波
//	Velocity_Low_Err = (1-a)*Velocity_Err + a*Velocity_Low_Err_last;
//	Velocity_Low_Err_last = Velocity_Low_Err;
	
	//1.计算实际速度
	Velocity_Err = (Encode_A + Encode_B);
	//2.对速度进行低通滤波
	Velocity_Low_Err = (1-a)*Velocity_Err + a*Velocity_Low_Err_last;
	Velocity_Low_Err_last = Velocity_Low_Err;	
	//3.计算速度偏差
	Velocity_Low_Err = Velocity_Low_Err - Med;
	//4.代入公式
	PWM_Out = Velocity_Kp * Velocity_Low_Err  + Velocity_Kd * (Acc - 0);
	
	return PWM_Out;
}

//PID函数
//串级PID最终结果
//公式:速度环Kp*真实速度 + Kd*速度偏差微分 - 速度环Kp*（位移环Kp*位移偏差 + Ki*位移偏差微分）
//输入:位移环输出结果、编码器AB相速度(用于求速度偏差)、真实加速度(当做速度偏差微分)
int Final_PWM_Out(float Displacement_PWM_Out, int16_t Encode_A, int16_t Encode_B, float Acc)
{
	static int PWM_Out, Velocity_reality, Velocity_Low_reality , Velocity_Low_reality_last;
	float a = 0.7;//低通滤波参数
	
	//1.计算实际速度
	Velocity_reality  = (Encode_A + Encode_B);
	//2.对速度进行低通滤波
	Velocity_Low_reality  = (1-a)*Velocity_reality + a*Velocity_Low_reality_last;
	Velocity_Low_reality_last = Velocity_Low_reality ;	
	//3.计算串级PID最终结果
	PWM_Out = Velocity_Kp*Velocity_Low_reality + Velocity_Kd*Acc - Velocity_Kp*Displacement_PWM_Out;
	
	//或许还要最后一次限幅（未写）
	
	return PWM_Out;
}

void Set_Med_Displacement(float* Med_Displacement)
{
	switch(Achievement)
	{
		case 0://去A
		{
			*Med_Displacement = 6;
			break;
		}
		case 1://返回起点，转弯
		{
			*Med_Displacement = 170;
			//转弯
			//Motor_PWMLoad(&PWM_Out, &PWM_Out);
			//delay()
			//停转			
			break;
		}
		case 2://去B
		{
			*Med_Displacement = 6;
			break;
		}
		case 3://倒退15，转弯
		{
			*Med_Displacement = 15;
			break;
		}
		case 4://去C
		{
			*Med_Displacement = 6;
			break;
		}
		case 5://倒退55，回到BC垂直交点处转弯，车头对着B
		{
			*Med_Displacement = 55;
			break;
		}
		case 6://返回起点
		{
			*Med_Displacement = 170;
			break;
		}
	}

}

#include "stm32f10x.h"                  // Device header

extern uint8_t Achievement;

//机械中值
float Med_Displacement = 0;

//外环位移环参数
float 
	Displacement_Kp = 0,
	Displacement_Ki = 0;

//内环速度环参数(可以使用结构体，但我这里不用)
float 
	Velocity_Kp = 0, 
	Velocity_Kd = 0;


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
int Velocity(float Med, float Encode_A, float Encode_B, float Acc)
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
	Velocity_Err = Velocity_Err - Med;
	//4.代入公式
	PWM_Out = Velocity_Kp * Velocity_Low_Err  + Velocity_Kd * (Acc - 0);
	
	return PWM_Out;
}

//PID函数
//串级PID最终结果
//公式:速度环Kp*真实速度 + Kd*速度偏差微分 - 速度环Kp*（位移环Kp*位移偏差 + Ki*位移偏差微分）
//输入:位移环输出结果、编码器AB相速度(用于求速度偏差)、真实加速度(当做速度偏差微分)
int Final_PWM_Out(float Displacement_PWM_Out, float Encode_A, float Encode_B, float Acc)
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

void Set_Med_Displacement(int* Med_Displacement)
{
	switch(Achievement)
	{
		case 0://去A
		{
			*Med_Displacement = 5;
			break;
		}
		case 1://返回起点，转弯
		{
			*Med_Displacement = 100;
			break;
		}
		case 2://去B
		{
			*Med_Displacement = 5;
			break;
		}
		case 3://倒退15，转弯
		{
			*Med_Displacement = 15;
			break;
		}
		case 4://去C
		{
			*Med_Displacement = 5;
			break;
		}
		case 5://倒退55，回到BC垂直交点处转弯，车头对着B
		{
			*Med_Displacement = 55;
			break;
		}
		case 6://返回起点
		{
			*Med_Displacement = 100;
			break;
		}
	}

}
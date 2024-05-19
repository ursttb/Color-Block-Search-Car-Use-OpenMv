#include "motor.h"


#include	"delay.h"
void motor_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	
	//����GPIOBʱ�ӣ��Լ���ʼ�����Ƶ������ת��IO��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 ;

//	GPIO_InitStruct.GPIO_Pin=AIN1|AIN2|BIN2|BIN1;			//�������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;		//�������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;		//�������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

             
	
}

//���Ʒ���
void Limit(int *MOTOA,int *MOTOB)
{
		if(*MOTOA>PWM_MAX)	*MOTOA=PWM_MAX;
		if(*MOTOA<PWM_MIN)	*MOTOA=PWM_MIN;
	
		if(*MOTOB>PWM_MAX)	*MOTOB=PWM_MAX;
		if(*MOTOB<PWM_MIN)	*MOTOB=PWM_MIN;

}
//����ֵ����
int GFP_abs (int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

//��ֵ����
void Load(int moto1,int moto2)
{	

	//������ģ�ֻ��moto1<0��ʱ�򣬵��ת��>0���ûת

////	if(moto1>0)	{AIN1=1,AIN2=0,LED2=1;}
////	else				{AIN1=0,AIN2=1,LED2=0;}//ROLL<-3.0f
//	TIM_SetCompare3(TIM2,GFP_abs(moto2));
if(moto1>0)	{GPIO_ResetBits(GPIOB,GPIO_Pin_14),GPIO_SetBits(GPIOB,GPIO_Pin_15);}//
else				{GPIO_SetBits(GPIOB,GPIO_Pin_14),GPIO_ResetBits(GPIOB,GPIO_Pin_15);}
TIM_SetCompare3(TIM2,GFP_abs(moto1));
	
		if(moto2>0)	{GPIO_SetBits(GPIOB,GPIO_Pin_12),GPIO_ResetBits(GPIOB,GPIO_Pin_13);}//
	else				{GPIO_ResetBits(GPIOB,GPIO_Pin_12),GPIO_SetBits(GPIOB,GPIO_Pin_13);}
	TIM_SetCompare4(TIM2,GFP_abs(moto2));
//	
	//û����ĵ�������߶�����ת
//	if(moto2>0)	{BIN1=1,BIN2=0;}
//	else				{BIN1=0,BIN2=1;}//ROLL<-3.0
//	TIM_SetCompare4(TIM2,GFP_abs(moto2));

}
char PWM_Zero=0,stop=0;

void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(GFP_abs(*Jiaodu-*Med_Jiaodu)>35)
	Load(PWM_Zero,PWM_Zero);
}
















//void motor_mode(u16 number)
//{
//	
//	if(number == 1)           //�����������ǰ��
//	{
//		GPIO_SetBits(GPIOB,AIN2);
//	  GPIO_ResetBits(GPIOB,AIN1);
//		GPIO_SetBits(GPIOB,BIN1);
//	  GPIO_ResetBits(GPIOB,BIN2);
//	}
//	if(number == 2)						//���������������
//	{	

//		GPIO_SetBits(GPIOB,AIN1);
//	  GPIO_ResetBits(GPIOB,AIN2);
//		GPIO_SetBits(GPIOB,BIN2);
//	  GPIO_ResetBits(GPIOB,BIN1);
//	}
//	if(number == 3)						//�������������ת
//	{	
//		GPIO_SetBits(GPIOB,AIN2);
//	  GPIO_ResetBits(GPIOB,AIN1);
//		GPIO_SetBits(GPIOB,BIN1);
//	  GPIO_ResetBits(GPIOB,BIN2);
//	}
//		if(number == 4)						//�������������ת
//	{	
//		GPIO_SetBits(GPIOB,AIN2);
//	  GPIO_ResetBits(GPIOB,AIN1);
//		GPIO_SetBits(GPIOB,BIN1);
//	  GPIO_ResetBits(GPIOB,BIN2);
//	}
//	if(number == 5)						//�������������ֹ
//	{
//		GPIO_ResetBits(GPIOB,AIN1);
//		GPIO_ResetBits(GPIOB,AIN2);
//		GPIO_ResetBits(GPIOB,BIN1);
//		GPIO_ResetBits(GPIOB,BIN2);
//	}
//	




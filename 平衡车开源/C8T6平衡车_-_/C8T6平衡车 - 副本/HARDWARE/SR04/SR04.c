#include  "sys.h"
#include  "delay.h"
#include  "SR04.h"
#include "encoder.h"

//������ģ�����Ŷ���
#define Trig GPIO_Pin_11
#define Echo GPIO_Pin_8
#define GPIO_HR_SR04 GPIOA

u16 Distance;
u16 Count = 0;
u16 TIM1CH1_CAPTURE_STA,TIM1CH1_CAPTURE_VAL;
u16 UltrasonicWave_Distance;



void SR04_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;


	TIM_ICInitTypeDef  TIM_ICInitStruct;
//	EXTI_InitTypeDef  EXTI_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��IO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//�ⲿ�ж�EXTIʱ���ߴ�
	
	
	
	GPIO_InitStruct.GPIO_Pin=Trig;								//�����ź�
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;		//�������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_HR_SR04,&GPIO_InitStruct);
	
//	GPIO_ResetBits(GPIO_HR_SR04,Trig);            //���ʹ����źŵ�ƽ
	//GPIO_SetBits(GPIO_HR_SR04,Trig);
	
	
	GPIO_InitStruct.GPIO_Pin=Echo;								//�����ź�
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;			//�������� 
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_HR_SR04,&GPIO_InitStruct);
	
	//�ⲿ�ж�
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	
//	GPIO_ResetBits(GPIO_HR_SR04,Echo);						//���ͻ����źŵ�ƽ
	//GPIO_SetBits(GPIO_HR_SR04,Echo);

	//�ⲿ�жϳ�ʼ������
//	EXTI_InitStruct.EXTI_Line=EXTI_Line0;
//	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
//	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//�½��ش���
//	EXTI_Init(&EXTI_InitStruct);
	

	//��ʱ��1�ĳ�ʼ�����ж�
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period=65536-1;											//Ԥ��������װ�ص����ã�1ms��һ��
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
  
		
	//TIM_Cmd(TIM3,DISABLE);
	

	
	//��ʱ�����벶����
		TIM_ICStructInit(&TIM_ICInitStruct);
		TIM_ICInitStruct.TIM_Channel = TIM_Channel_1; 									//PB0��ӦTIM3��ͨ��3
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;			//�����ز���
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI2��
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 				//Ԥ���䣨�����з�Ƶ��
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;											//�����˲��� ����
  	TIM_ICInit(TIM1, &TIM_ICInitStruct);
		
		
	//NVIC�ж����ȼ�����

	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);   //������ʱ�������ж������벶���ж�
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	
	TIM_Cmd(TIM1,ENABLE);
}	


////��ʱ��1���жϷ�����
void TIM1_CC_IRQHandler(void)
{
	if((TIM1CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ��������	
{	  
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
		{	    
			if(TIM1CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
				{
					if((TIM1CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
					{
						TIM1CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
						TIM1CH1_CAPTURE_VAL=0XFFFF;
					}
					else TIM1CH1_CAPTURE_STA++;
				}	 
		}
	  
		
		if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)//����3���������¼�
		{	
			if(TIM1CH1_CAPTURE_STA&0X40)		//����͵�ƽ 		
			{	  			
				TIM1CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ
				TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);
		   		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM1CH1_CAPTURE_STA=0;			//���
				TIM1CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM1,0);
				TIM1CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   	TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//����Ϊ�½��ز���
			}		    
		}			     	    					   
 }
 
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}


//void EXTI0_IRQHandler(void)
//{

//	if(EXTI_GetITStatus(EXTI_Line0) != RESET )
//	{
//		EXTI_ClearITPendingBit(EXTI_Line0);
//	
//		TIM_SetCounter(TIM3,0);
//		TIM_Cmd(TIM3,ENABLE);
//		
//		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));  //?????
//		
//		TIM_Cmd(TIM3,DISABLE);
//		Distance=TIM_GetCounter(TIM3)*340/200.0;
//	
//	}
//}



//��ȡ��ʱ��3������ֵ
u32 GetEchoTimer(void)
{
   u32 t = 0;
   t = Count*1000;
   t += TIM_GetCounter(TIM1);
   TIM1->CNT = 0;  
   delay_ms(50);
   return t;
}

float Read_TIM1Distane()
{   


	GPIO_SetBits(GPIO_HR_SR04,Trig);
	delay_us(20);  
	GPIO_ResetBits(GPIO_HR_SR04,Trig);

	
	if(TIM1CH1_CAPTURE_STA&0X80)
	{
		Distance=TIM1CH1_CAPTURE_STA&0X3F;
		Distance*=65536;					        
		Distance+=TIM1CH1_CAPTURE_VAL;		
//		Distance = Distance*170/1000;
//		printf("%d \r\n",Distance);
		TIM1CH1_CAPTURE_STA = 0;
	}
	return Distance;

}



////ͨ����ʱ��1������ֵ�������
//u16 GetLength(void)
//{

//   u32 t = 0;
//   int i=0;
//	float leng = 0;
//   float sum = 0;


//   while(i!=5)
//	 {while(GPIO_ReadInputDataBit(GPIO_HR_SR04,Echo)==1);//�жϻ����ź��Ƿ��Ǹߵ�ƽ
//		GPIO_SetBits(GPIO_HR_SR04,Trig);	//�����������øߵ�ƽ��Ȼ����ʱ10us����
//		delay_us(20);
//		GPIO_ResetBits(GPIO_HR_SR04,Trig);
//			
//		 //�������ź��øߵ�ƽ���Ϳ�ʼ��ʱ��3������
//		  while(GPIO_ReadInputDataBit(GPIO_HR_SR04,Echo)==0); 
//			TIM_SetCounter(TIM1,0);
//		  Count=0;
//		  TIM_Cmd(TIM1,ENABLE);
//		  i=i+1;
//		 
//				while(GPIO_ReadInputDataBit(GPIO_HR_SR04,Echo)==1);
//				TIM_Cmd(TIM1,DISABLE);
//				t=GetEchoTimer();
//		 
//		leng = ((float)t/58.0);//cm
//     sum += leng;
//        
//	 }
//    leng = sum/5.0;
//    return leng;
//}













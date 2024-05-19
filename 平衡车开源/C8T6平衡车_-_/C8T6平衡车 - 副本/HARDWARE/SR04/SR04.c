#include  "sys.h"
#include  "delay.h"
#include  "SR04.h"
#include "encoder.h"

//超声波模块引脚定义
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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能IO口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//外部中断EXTI时钟线打开
	
	
	
	GPIO_InitStruct.GPIO_Pin=Trig;								//触发信号
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_HR_SR04,&GPIO_InitStruct);
	
//	GPIO_ResetBits(GPIO_HR_SR04,Trig);            //拉低触发信号电平
	//GPIO_SetBits(GPIO_HR_SR04,Trig);
	
	
	GPIO_InitStruct.GPIO_Pin=Echo;								//回响信号
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;			//下拉输入 
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_HR_SR04,&GPIO_InitStruct);
	
	//外部中断
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	
//	GPIO_ResetBits(GPIO_HR_SR04,Echo);						//拉低回响信号电平
	//GPIO_SetBits(GPIO_HR_SR04,Echo);

	//外部中断初始化配置
//	EXTI_InitStruct.EXTI_Line=EXTI_Line0;
//	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
//	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//下降沿触发
//	EXTI_Init(&EXTI_InitStruct);
	

	//定时器1的初始化与中断
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=65536-1;											//预分配与重装载的配置，1ms数一次
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
  
		
	//TIM_Cmd(TIM3,DISABLE);
	

	
	//定时器输入捕获定义
		TIM_ICStructInit(&TIM_ICInitStruct);
		TIM_ICInitStruct.TIM_Channel = TIM_Channel_1; 									//PB0对应TIM3的通道3
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;			//上升沿捕获
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI2上
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 				//预分配（不进行分频）
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;											//配置滤波器 不滤
  	TIM_ICInit(TIM1, &TIM_ICInitStruct);
		
		
	//NVIC中断优先级设置

	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);   //开启定时器更新中断与输入捕获中断
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	
	TIM_Cmd(TIM1,ENABLE);
}	


////定时器1的中断服务函数
void TIM1_CC_IRQHandler(void)
{
	if((TIM1CH1_CAPTURE_STA&0X80)==0)//还未成功捕获完成	
{	  
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
		{	    
			if(TIM1CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM1CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
					{
						TIM1CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
						TIM1CH1_CAPTURE_VAL=0XFFFF;
					}
					else TIM1CH1_CAPTURE_STA++;
				}	 
		}
	  
		
		if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)//捕获3发生捕获事件
		{	
			if(TIM1CH1_CAPTURE_STA&0X40)		//捕获低电平 		
			{	  			
				TIM1CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平
				TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);
		   		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}
			else  								//还未开始,第一次捕获上升沿
			{
				TIM1CH1_CAPTURE_STA=0;			//清空
				TIM1CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM1,0);
				TIM1CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   	TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//设置为下降沿捕获
			}		    
		}			     	    					   
 }
 
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
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



//获取定时器3计数器值
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



////通过定时器1计数器值推算距离
//u16 GetLength(void)
//{

//   u32 t = 0;
//   int i=0;
//	float leng = 0;
//   float sum = 0;


//   while(i!=5)
//	 {while(GPIO_ReadInputDataBit(GPIO_HR_SR04,Echo)==1);//判断回响信号是否是高电平
//		GPIO_SetBits(GPIO_HR_SR04,Trig);	//将触发引脚置高电平，然后延时10us以上
//		delay_us(20);
//		GPIO_ResetBits(GPIO_HR_SR04,Trig);
//			
//		 //当回响信号置高电平，就开始定时器3并计数
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













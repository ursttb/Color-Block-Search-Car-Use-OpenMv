#include "stm32f10x.h"                  // Device header
#include "SYS.h"

uint8_t Achievement = 0;

//ת���ٶ�
int motor1 = 7200; int motor2 = -7200;

void Mode_Car()
{
	switch(Mode)
	{
		case 1:
		{
			//������
			break;
		}
		case 2:	//ת��
		{
			turn(turn_dir,15);
			Mode = 1;
			TIM_Cmd(PID_TIM,ENABLE); 
			break;
		}
		case 3:
		{
			//������
			if(Distance<=6 && Redflag == 1)
			{
				OLED_ShowString(4,5,"YES");//�������� 
				
				Achievement++;
				Set_Med_Displacement(&Med_Displacement);
	 		}
			break;
		}	
		
	}
	
}

int main()
{
	OLED_Init();
	
	USARTx_Init();
	
	MPU6050_Init();
	Encoder_Init();
	PWM_Init();
	Motot_Init();

	EXIT_Init();
	PID_Init();                                                                                                                                                                                                       

	Set_Med_Displacement(&Med_Displacement);
	while(1)
	{
		static uint16_t i = 0;
//		i++;
//		if(i>bgb=8000)
//		{
			i = 0;
			OLED_ShowNum(1,1,AX,2);	OLED_ShowNum(1,4,Mode,2);  OLED_ShowSignedNum(1,7,turn_dir,2);		
			OLED_ShowSignedNum(2,1,Encoder_left,3);
			OLED_ShowSignedNum(2,6,Encoder_right,3);
			OLED_ShowNum(3, 1,Distance,3);		//��ʾ��λΪ���׵ľ�����
			OLED_ShowNum(4,1,PWM_Out,4);
			OLED_ShowNum(4,6,Red_x,2); OLED_ShowNum(4,9,Achievement,2);
//		}
		Mode_Car();
	}
 	return 0;
} 

#ifndef __TRACKING_H__
#define __TRACKING_H__
#include "sys.h"

#define Right_tracking  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//�ұߵĺ���ѭ��ģ��
#define Left_tracking   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//��ߵĺ���ѭ��ģ���Ӧ��out��

void Tracking_Init(void);
void Tracking_Mode(void);



 





#endif 


